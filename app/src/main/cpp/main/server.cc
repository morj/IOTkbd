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

#include "user.h"
#include "fatal_assert.h"
#include "networktransport-impl.h"
#include "select.h"
#include "common.h"

#include <memory>

using namespace Network;

void runServer()
{
  UserStream me, remote;

  auto n = std::make_unique<Transport<UserStream, UserStream>>(me, remote, nullptr, "1337");
  LOGE("Port bound is %s, key is %s\n", n->port().c_str(), n->get_key().c_str());
 
  n->set_verbose();

  Select &sel = Select::get_instance();
  uint64_t last_num = n->get_remote_state_num();
  while (true) {
    try {
      sel.clear_fds();
      std::vector<int> fd_list(n->fds());
      assert(fd_list.size() == 1); /* servers don't hop */
      int network_fd = fd_list.back();
      sel.add_fd(network_fd);
      if (sel.select(n->wait_time()) < 0) {
        perror("select");
        exit(1);
      }

      n->tick();

      if (sel.read(network_fd)) {
        n->recv();

        if (n->get_remote_state_num() != last_num) {
          Network::UserStream us;
          us.apply_string( n->get_remote_diff() );

          for (auto &&event : *us.a()) {
            const char *buf = event.userbyte.c;
            LOGE("%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", buf[0], buf[1], buf[2], buf[3],
                 buf[4], buf[5], buf[6], buf[7]);
          }

          LOGE("--- %d\n", (int)n->get_latest_remote_state().state.size());

          last_num = n->get_remote_state_num();
        }
      }
    } catch (const std::exception &e) {
      LOGE("Server error: %s\n", e.what());
    }
  }

}

int main(int argc, char *argv[]) {
  try {
   runServer();
  } catch (const std::exception &e) {
    LOGE("Fatal startup error: %s\n", e.what());
    exit(1);
  }
}
