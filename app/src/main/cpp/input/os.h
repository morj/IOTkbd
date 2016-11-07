/*
    ckb: RGB Driver for Linux and OS X
    Copyright https://github.com/ccMSC/ckb

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OS_H
#define OS_H

// OS definitions

#ifdef __linux
#define OS_LINUX
#endif
#ifdef __APPLE__
#define OS_MAC
#endif

#if !defined(OS_LINUX) && !defined(OS_MAC)
#error Your OS is not supported. Edit os.h if you want to compile anyway.
#endif

// OS-specific includes

#ifdef OS_LINUX

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <features.h>
#include <linux/uinput.h>
#include <linux/usbdevice_fs.h>

#ifndef UINPUT_VERSION
#define UINPUT_VERSION 2
#endif

// The OSX process needs to change its EUID to post events, so thread safety must be ensured
// On Linux the EUID is always root
#define euid_guard_start
#define euid_guard_stop

#endif  // OS_LINUX

#ifdef OS_MAC

#include <Carbon/Carbon.h>
#include <IOKit/IOMessage.h>
#include <IOKit/hid/IOHIDDevicePlugin.h>
#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/hidsystem/IOHIDLib.h>
#include <IOKit/hidsystem/ev_keymap.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/usb/USB.h>

typedef IOHIDDeviceDeviceInterface**    hid_dev_t;
typedef IOUSBDeviceInterface182**       usb_dev_t;
typedef IOUSBInterfaceInterface183**    usb_iface_t;

// The OSX process needs to change its EUID to post events, so thread safety must be ensured
// On Linux the EUID is always root
extern pthread_mutex_t _euid_guard;
#define euid_guard_start    pthread_mutex_lock(&_euid_guard)
#define euid_guard_stop     pthread_mutex_unlock(&_euid_guard)

// Various POSIX functions that aren't present on OSX

void *memrchr(const void *s, int c, size_t n);

#define CLOCK_MONOTONIC 1
#define TIMER_ABSTIME   1
typedef int clockid_t;
int clock_gettime(clockid_t clk_id, struct timespec *tp);
int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *rqtp, struct timespec *rmtp);

#endif  // OS_MAC

#endif  // OS_H
