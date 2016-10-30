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

#ifndef IOTKBD_DEVICE_H
#define IOTKBD_DEVICE_H

#include <jni.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <select.h>
#include <network.h>
#include <user.h>
#include <networktransport.h>

using namespace Network;

int createSocket(sockaddr_in &si_other, socklen_t &slen);

namespace System {

   /* class Device {
    private:
        // static const int CONSTANT = 1112123;
        jint fd;

    public:
        Device(jint fd) : fd(fd) {
        }

        ~Device() {
        }

        // Device &operator=(const Device &);
        *//* effective limit on terminal size *//*



        int getFeature(unsigned char reportId, unsigned char *buffer, int length);

        int setFeature(unsigned char reportId, unsigned char *buffer, int length);
    };

    class State {
    private:
        jint fd;
        Select &sel;
        Transport<UserStream, UserStream> *n;

    public:
        State(jint fd, Select &sel, Transport<UserStream, UserStream> *n) : fd(fd), sel(sel), n(n) {
        }

        ~State() {
        }
    };*/

    // Device &get_device(void);
}

#endif //IOTKBD_DEVICE_H
