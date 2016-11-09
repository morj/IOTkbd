/*
    IOTkbd: UDP-based wireless keyboard
    Copyright 2016 Pavel Nikitin

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

#include <jni.h>
#include <string>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>

#include <memory>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

#include "main.h"
#include "common.h"
#include "networktransport-impl.h"

// ~/Downloads/protoc-3.1.0-linux-x86_64.exe --proto_path=cpp --cpp_out=cpp/protobuf-generated cpp/protobufs/hostinput.proto cpp/protobufs/transportinstruction.proto cpp/protobufs/userinput.proto

using namespace System;
using namespace std;
using namespace Network;

class USBRequestBlock
{
  std::vector<char> buffer;
  usbdevfs_urb urb;
public:
  USBRequestBlock(int buffer_length, unsigned char endpoint, unsigned int signr) : buffer(
      buffer_length)
  {
    urb = {USBDEVFS_URB_TYPE_INTERRUPT, endpoint, 0, 0, buffer.data(), buffer_length, 0, 0, 0, 0,
           signr};
  }

  int submitSilently(int fd)
  {
    return ioctl(fd, USBDEVFS_SUBMITURB, &urb);
  }

  int submit(int fd)
  {
    int result = submitSilently(fd);
    LOGV("Ret: %d", result);
    LOGV("Err: %d", errno);
    return result;
  }

  unsigned int getSignr() const
  {
    return urb.signr;
  }
};

// #define WAIT_STEP 5
void notifyDeviceAttached(int fd, int endp)
{
  LOGV("Device attached: %d, endp: %d", fd, endp);

  Base64Key key("790RmsZ+DtKOGSeVqsS6DA");
  //Session session(key);

  UserStream me, remote;

  auto transport = std::make_unique<Transport<UserStream, UserStream>>(
      me, remote, key.printable_key().c_str(), SERVER, PORTS
  );


  LOGV("Inited");

  LOGV("Select inited");

  USBRequestBlock parent_urb({8, (unsigned char) endp, SIGUSR2});

  unsigned int signal = parent_urb.getSignr();
  Select::add_signal_s(signal);
  bool readKeyboard = false;
  if (parent_urb.submit(fd) < 0) {
    LOGV("no ioctl 1 :(");
  } else {
    LOGV("start loop");

    while (true) {
      Select::clear_fds_s();

      if (readKeyboard) {
        readKeyboard = false;
        parent_urb.submitSilently(fd);
      }
      std::vector<int> fd_list(transport->fds());
      std::for_each(fd_list.begin(), fd_list.end(), Select::add_fd_s);

      try {
        int timeout = transport->wait_time();
        if (timeout > 1000) {
          LOGV("Will wait for: %d", timeout);
        }
        /*int waited = 0;
        do {
          int control = signal;
          waited += WAIT_STEP;
          if (Select::select_s(control, waited > timeout ? (WAIT_STEP - (waited - timeout)) : WAIT_STEP) < 0) {
            perror("select");
          }
          readKeyboard = control == 1 || Select::signal_s(signal);
        } while (!readKeyboard && waited < timeout);*/

        int control = signal;
        if (Select::select_s(control, timeout)) {
          perror("select");
        }
        readKeyboard = control == 1 || Select::signal_s(signal);
      } catch (const std::exception &e) {
        LOGE("Client error: %d\n", 0); //e.what()
      }

      transport->tick();

      struct usbdevfs_urb *urb = 0;
      if (readKeyboard) {
        int iores = ioctl(fd, USBDEVFS_REAPURB, &urb);
        if (iores) {
          LOGV("Ioctl returns %d", iores);
          if (errno == ENODEV || errno == ENOENT || errno == ESHUTDOWN) {
            LOGV("Error: %d", errno);
            // Stop the thread if the handle closes
            break;
          } else if (errno == EPIPE && urb) {
            LOGV("Error: EPIPE");
            // On EPIPE, clear halt on the endpoint
            ioctl(fd, USBDEVFS_CLEAR_HALT, &urb->endpoint);
            // Re-submit the URB
            if (urb) {
              ioctl(fd, USBDEVFS_SUBMITURB, urb);
            }
            urb = 0;
          }
        }

        if (!urb) {
          LOGV("No urb");
        }
      }

      if (urb) {
        char *buf = (char *) (urb->buffer);

        //sprintf(message, "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
        //LOGV("%s", message);

        transport->get_current_state().push_back(Network::UserByte(buf));

        urb = 0;
      }

      if (std::any_of(fd_list.begin(), fd_list.end(), Select::read_s)) {
        LOGV("Read from network (local %d)", transport->get_current_state().size());
        transport->recv();
      }
    }
  }
}

extern "C" {
JNIEXPORT void JNICALL
Java_com_github_morj_iotkbd_MainActivity_notifyDeviceAttached(JNIEnv *env, jclass type, jint fd,
                                                              jint endp)
{
  try {
    notifyDeviceAttached(static_cast<int>(fd), static_cast<int>(endp));
  }
  catch (CryptoException e) {
    LOGV("Crypto exception: %s", e.text.c_str());
  }
}
}

extern "C" {
JNIEXPORT void JNICALL
Java_com_github_morj_iotkbd_MainActivity_notifyDeviceDetached(JNIEnv *env, jclass type, jint fd)
{
  LOGV("Device detached: %d", fd);
}
}

/*static inline uint16_t cpu_to_le16(const uint16_t x) {
  union {
    uint8_t b8[2];
    uint16_t b16;
  } _tmp;

  _tmp.b8[1] = (uint8_t) (x >> 8);
  _tmp.b8[0] = (uint8_t) (x & 0xff);

  return _tmp.b16;
}

struct usbdevfs_ctrltransfer
{
    unsigned char bRequestType;
    unsigned char bRequest;
    unsigned short wValue;
    unsigned short wIndex;
    unsigned short wLength;
    unsigned int timeout;
    void *data;
};

int Device::getFeature(unsigned char reportId, unsigned char *buffer, int length) {
    struct usbdevfs_ctrltransfer data;

    data.bRequestType = (0x01 << 5) | 0x01 | 0x80;
    data.bRequest = 0x01;
    data.wValue = cpu_to_le16((3 << 8) | reportId);
    data.wIndex = cpu_to_le16(0);
    data.wLength = cpu_to_le16(length);
    data.data = buffer;
    data.timeout = 1000;

    int res = ioctl(fd, _IOWR('U', 0, struct usbdevfs_ctrltransfer), &data);

    if (res < 0) {
        LOGV("Error invoking getFeature: %d", errno);
    }

    return res;
}

int Device::setFeature(unsigned char reportId, unsigned char *buffer, int length) {
    struct usbdevfs_ctrltransfer data;

    data.bRequestType = (0x01 << 5) | 0x01 | 0x00;
    data.bRequest = 0x09;
    data.wValue = cpu_to_le16((3 << 8) | reportId);
    data.wIndex = cpu_to_le16(0);
    data.wLength = cpu_to_le16(length);
    data.data = buffer;
    data.timeout = 1000;

    int res = ioctl(fd, _IOWR('U', 0, struct usbdevfs_ctrltransfer), &data);

    if (res < 0) {
        LOGV("Error invoking getFeature: %d", errno);
    }

    return res;
}*/
