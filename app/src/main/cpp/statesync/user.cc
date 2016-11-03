/*
    Mosh: the mobile shell
    Copyright 2012 Keith Winstein

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

#include <assert.h>
#include <typeinfo>

#include "user.h"
#include "fatal_assert.h"
#include "protobufs/userinput.pb.h"

using namespace Network;
using namespace ClientBuffers;

void UserStream::subtract(const UserStream *prefix) {
  // if we are subtracting ourself from ourself, just clear the deque
  if (this == prefix) {
    actions.clear();
    return;
  }
  for (deque<UserEvent>::const_iterator i = prefix->actions.begin();
       i != prefix->actions.end();
       i++) {
    assert(this != prefix);
    assert(!actions.empty());
    assert(*i == actions.front());
    actions.pop_front();
  }
}

string UserStream::diff_from(const UserStream &existing) const {
  deque<UserEvent>::const_iterator my_it = actions.begin();

  for (deque<UserEvent>::const_iterator i = existing.actions.begin();
       i != existing.actions.end();
       i++) {
    assert(my_it != actions.end());
    assert(*i == *my_it);
    my_it++;
  }

  ClientBuffers::UserMessage output;

  while (my_it != actions.end()) {
    switch (my_it->type) {
      case UserByteType: {
        const char *bytes = my_it->userbyte.c;
        Instruction *new_inst = output.add_instruction();
        new_inst->MutableExtension(keystroke)->set_keys(bytes, 8);
      }
        break;
      default:
        assert(false);
        break;
    }

    my_it++;
  }

  return output.SerializeAsString();
}

void UserStream::apply_string(const string &diff) {
  ClientBuffers::UserMessage input;
  fatal_assert(input.ParseFromString(diff));

  for (int i = 0; i < input.instruction_size(); i++) {
    if (input.instruction(i).HasExtension(keystroke)) {
      string the_bytes = input.instruction(i).GetExtension(keystroke).keys();
      actions.push_back(UserEvent(UserByte(the_bytes.data())));
    }
  }
}
