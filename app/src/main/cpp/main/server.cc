/*
    IOTkbd: UDP-based wireless keyboard
    Copyright 2016 Pavel Nikitin
    Based on ckb: RGB Driver for Linux and OS X
    Copyright https://github.com/ccMSC/ckb

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    In addition, as a special exception, the copyright holders give
    permission to link the code of portions of this program with the
    OpenSSL library under certain conditions as described in each
    individual source file, and distribute linked combinations including
    the two.

    You must obey the GNU General Public License in all respects for all
    of the code used other than OpenSSL. If you modify file(s) with this
    exception, you may extend this exception to your version of the
    file(s), but you are not obligated to do so. If you do not wish to do
    so, delete this exception statement from your version. If you delete
    this exception statement from all source files in the program, then
    also delete it here.
*/

#include "user.h"
#include "fatal_assert.h"
#include "input-common.h"
#include "networktransport-impl.h"
#include "select.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include <memory>

#include "keymap.h"

using namespace Network;

int global_fd = -1;

void clearInput(int fd);

void exitHandler(int s)
{
  printf("Caught signal %d, fd was: %d\n", s, global_fd);
  if (global_fd != -1) {
    clearInput(global_fd);
    ioctl(global_fd, UI_DEV_DESTROY);
    close(global_fd);
    global_fd = -1;
  }
  exit(1);
}

void addExitHandler()
{
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = exitHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);
}

int createVirtualKeyboard()
{
  int fd = open("/dev/uinput", O_RDWR);
  if (fd < 0) {
    fd = open("/dev/input/uinput", O_RDWR); // try /dev/input/uinput instead
  }
  if (fd <= 0) {
    LOGE("Can't open uinput device");
    return -1;
  }
  if (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0) {
    LOGW("uinput evbit failed: %s\n", strerror(errno));
    return -1;
  }

  for (int i = 0; i < KEY_CNT; i++) {
    ioctl(fd, UI_SET_KEYBIT, i);
  }

  // enable LED control
  ioctl(fd, UI_SET_EVBIT, EV_LED);
  for (int i = 0; i < LED_CNT; i++) {
    ioctl(fd, UI_SET_LEDBIT, i);
  }
  // enable auto-repeat
  ioctl(fd, UI_SET_EVBIT, EV_REP);
  // enable synchronization
  ioctl(fd, UI_SET_EVBIT, EV_SYN);

  struct uinput_user_dev indev;
  memset(&indev, 0, sizeof(uinput_user_dev));
  snprintf(indev.name, UINPUT_MAX_NAME_SIZE, "iotkbd 001");
  indev.id.bustype = BUS_USB;
  indev.id.vendor = 9494;
  indev.id.product = 23;
  indev.id.version = 0;

  if (write(fd, &indev, sizeof(indev)) <= 0) {
    LOGW("uinput create write failed: %s\n", strerror(errno));
    return -1;
  } else {
    if (ioctl(fd, UI_DEV_CREATE) < 0) {
      LOGW("uinput create ioctl failed: %s\n", strerror(errno));
      return -1;
    }
  }
  return fd;
}

void emitKeypress(int fd, int scancode, int down)
{
  struct input_event event;
  memset(&event, 0, sizeof(event));
  event.type = EV_KEY;
  event.code = (__u16) (scancode & ~SCAN_MOUSE);
  event.value = down;
  if (write(fd, &event, sizeof(event)) <= 0) {
    LOGW("uinput write failed: %s\n", strerror(errno));
  }
}

void syncInput(int fd)
{
  struct input_event event;
  memset(&event, 0, sizeof(event));
  event.type = EV_SYN;
  event.code = SYN_REPORT;
  if (write(fd, &event, sizeof(event)) <= 0) {
    LOGW("uinput write failed: %s\n", strerror(errno));
  }
}

void clearInput(int fd)
{
  if (fd <= 0) {
    return;
  }

  struct input_event event;
  memset(&event, 0, sizeof(event));
  event.type = EV_KEY;
  for (int key = 0; key < KEY_CNT; key++) {
    event.code = (__u16) key;
    if (write(fd, &event, sizeof(event)) <= 0) {
      LOGW("uinput write failed: %s\n", strerror(errno));
    }
  }
  event.type = EV_SYN;
  event.code = SYN_REPORT;
  if (write(fd, &event, sizeof(event)) <= 0) {
    LOGW("uinput write failed: %s\n", strerror(errno));
  }
}

void emitKeyboard(int fd, unsigned char *keybytes, unsigned char *prevkeybytes,
                  const unsigned char *buf)
{
  hid_kb_translate(keybytes, -1, 8, buf);
  if (!memcmp(prevkeybytes, keybytes, N_KEYBYTES_INPUT)) {
    LOGD("Same kbd state\n");
    return;
  }

  int events[N_KEYS_INPUT + 4];
  int modcount = 0, keycount = 0, rmodcount = 0;
  for (int byte = 0; byte < N_KEYBYTES_INPUT; byte++) {
    char oldb = prevkeybytes[byte], newb = keybytes[byte];
    if (oldb == newb)
      continue;
    for (int bit = 0; bit < 8; bit++) {
      int keyindex = byte * 8 + bit;
      if (keyindex >= N_KEYS_INPUT)
        break;
      const key *map = keymap + keyindex;
      int scancode = map->scan;
      char mask = 1 << bit;
      char old = oldb & mask, newc = newb & mask;
      // If the key state changed, send it to the input device
      if (old != newc) {
        // Don't echo a key press if a macro was triggered or if there's no scancode associated
        if (!(scancode & SCAN_SILENT)) {
          if (IS_MOD(scancode)) {
            if (newc) {
              // Modifier down: Add to the end of modifier keys
              for (int i = keycount + rmodcount; i > 0; i--)
                events[modcount + i] = events[modcount + i - 1];
              // Add 1 to the scancode because A is zero on OSX
              // Positive code = keydown, negative code = keyup
              events[modcount++] = scancode + 1;
            } else {
              // Modifier up: Add to the end of everything
              events[modcount + keycount + rmodcount++] = -(scancode + 1);
            }
          } else {
            // Regular keypress: add to the end of regular keys
            for (int i = rmodcount; i > 0; i--)
              events[modcount + keycount + i] = events[modcount + keycount + i - 1];
            events[modcount + keycount++] = newc ? (scancode + 1) : -(scancode + 1);
          }
        }
      }
    }
  }
  // Process all queued keypresses
  int totalkeys = modcount + keycount + rmodcount;
  for (int i = 0; i < totalkeys; i++) {
    int scancode = events[i];
    emitKeypress(fd, (scancode < 0 ? -scancode : scancode) - 1, scancode > 0);
  }
  // LOGD("Total keys: %d\n", totalkeys);
}

void runServer(int kbd)
{
  UserStream me, remote;

  auto n = std::make_unique<Transport<UserStream, UserStream>>(me, remote, nullptr, "1337");
  LOGE("Port bound is %s, key is %s\n", n->port().c_str(), n->get_key().c_str());
  unsigned char keybytes[N_KEYBYTES_EXTENDED];
  memset(keybytes, 0, sizeof(unsigned char) * N_KEYBYTES_EXTENDED);
  unsigned char prevkeybytes[N_KEYBYTES_EXTENDED];
  memset(prevkeybytes, 0, sizeof(unsigned char) * N_KEYBYTES_EXTENDED);

  /*unsigned char b1[8] = {0, 0, 0x18, 0x0C, 0, 0, 0, 0};
  emitKeyboard(kbd, keybytes, prevkeybytes, b1);
  memcpy(prevkeybytes, keybytes, N_KEYBYTES_INPUT);
  syncInput(kbd);

  unsigned char empty[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  emitKeyboard(kbd, keybytes, prevkeybytes, empty);
  memcpy(prevkeybytes, keybytes, N_KEYBYTES_INPUT);
  syncInput(kbd);*/

  n->set_verbose();

  clearInput(kbd);

  Select &sel = Select::get_instance();
  uint64_t last_num = n->get_remote_state_num();
  int nothing = -1;
  while (true) {
    try {
      sel.clear_fds();
      std::vector<int> fd_list(n->fds());
      assert(fd_list.size() == 1); /* servers don't hop */
      int network_fd = fd_list.back();
      sel.add_fd(network_fd);
      if (sel.select(nothing, n->wait_time()) < 0) {
        perror("select");
        exit(1);
      }

      n->tick();

      if (sel.read(network_fd)) {
        n->recv();

        if (n->get_remote_state_num() != last_num) {
          Network::UserStream us;
          us.apply_string(n->get_remote_diff());

          for (auto &&event : *us.a()) {
            const unsigned char *buf = event.userbyte.c;
            emitKeyboard(kbd, keybytes, prevkeybytes, buf);
            memcpy(prevkeybytes, keybytes, N_KEYBYTES_INPUT);
          }
          syncInput(kbd);

          for (auto &&event : *us.a()) {
            const unsigned char *buf = event.userbyte.c;
            LOGD("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", buf[0], buf[1], buf[2], buf[3],
                 buf[4], buf[5], buf[6], buf[7]);
          }

          LOGD("--- %d\n", (int) n->get_latest_remote_state().state.size());

          last_num = n->get_remote_state_num();
        }
      }

      if (!n->has_remote_addr()) {
        clearInput(kbd);
      }
    } catch (const std::exception &e) {
      LOGE("Server error: %s\n", e.what());
    }
  }

}

int main(int argc, char *argv[])
{
  addExitHandler();

  int fd = createVirtualKeyboard();
  global_fd = fd;

  try {
    runServer(fd);
  } catch (const std::exception &e) {
    LOGE("Fatal startup error: %s\n", e.what());
    exit(1);
  }
}
