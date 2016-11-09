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

#ifndef IOTKBD_COMMON_H
#define IOTKBD_COMMON_H

#define APPNAME "IOTKBD"
#define SERVER "192.168.178.50" // netcat -ul 1337
// #define BUFLEN 512  //Max length of buffer
#define PORT 1337   //The port on which to send data
#define PORTS "1337"   //The port on which to send data

#ifdef __ANDROID_API__

#include <android/log.h>

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, APPNAME, __VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN, APPNAME, __VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, APPNAME, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, APPNAME, __VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, __VA_ARGS__)
#else
#include <stdio.h>
#define  LOGE(...)  fprintf( stderr, __VA_ARGS__)
#define  LOGW(...)  fprintf( stderr, __VA_ARGS__)
#define  LOGD(...)  fprintf( stderr, __VA_ARGS__)
#define  LOGI(...)  fprintf( stderr, __VA_ARGS__)
#define  LOGV(...)  fprintf( stderr, __VA_ARGS__)
#endif

#endif //IOTKBD_COMMON_H
