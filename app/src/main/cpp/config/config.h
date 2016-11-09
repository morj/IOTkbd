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

#ifndef IOTKBD_CONFIG_H
#define IOTKBD_CONFIG_H

//#undef ANDROID
//#define ANDROID

#endif //IOTKBD_CONFIG_H

#undef __SSE2__

/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if FD_ISSET() fd_set argument is const. */
#undef FD_ISSET_IS_CONST

/* Define if libutil.h necessary for forkpty(). */
#undef FORKPTY_IN_LIBUTIL

/* Define to 1 if you have the `cfmakeraw' function. */
#undef HAVE_CFMAKERAW

/* Define if clock_gettime() is available. */
#undef HAVE_CLOCK_GETTIME

/* Define to 1 if you have the <CommonCrypto/CommonCrypto.h> header file. */
#undef HAVE_COMMONCRYPTO_COMMONCRYPTO_H

/* Define to 1 if a SysV or X/Open compatible Curses library is present */
#undef HAVE_CURSES

/* Define to 1 if library supports color (enhanced functions) */
#undef HAVE_CURSES_COLOR

/* Define to 1 if library supports X/Open Enhanced functions */
#undef HAVE_CURSES_ENHANCED

/* Define to 1 if <curses.h> is present */
#undef HAVE_CURSES_H

/* Define to 1 if library supports certain obsolete features */
#undef HAVE_CURSES_OBSOLETE

/* Define to 1 if you have the declaration of `be64toh', and to 0 if you
   don't. */
#undef HAVE_DECL_BE64TOH

/* Define to 1 if you have the declaration of `betoh64', and to 0 if you
   don't. */
#undef HAVE_DECL_BETOH64

/* Define to 1 if you have the declaration of `bswap64', and to 0 if you
   don't. */
#undef HAVE_DECL_BSWAP64

/* Define to 1 if you have the declaration of `ffs', and to 0 if you don't. */
#undef HAVE_DECL_FFS

/* Define to 1 if you have the declaration of `__builtin_bswap64', and to 0 if
   you don't. */
#undef HAVE_DECL___BUILTIN_BSWAP64

/* Define to 1 if you have the declaration of `__builtin_ctz', and to 0 if you
   don't. */
#undef HAVE_DECL___BUILTIN_CTZ

/* Define to 1 if you have the <endian.h> header file. */
#undef HAVE_ENDIAN_H

/* Define to 1 if you have the <fcntl.h> header file. */
#undef HAVE_FCNTL_H

/* Define to 1 if you have the `fork' function. */
#undef HAVE_FORK

/* Define if you have forkpty(). */
#undef HAVE_FORKPTY

/* Define to 1 if you have the `getaddrinfo' function. */
#undef HAVE_GETADDRINFO

/* Define to 1 if you have the `getnameinfo' function. */
#undef HAVE_GETNAMEINFO

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define if IP_MTU_DISCOVER is a valid sockopt. */
#undef HAVE_IP_MTU_DISCOVER

/* Define if IP_RECVTOS is a valid sockopt. */
#undef HAVE_IP_RECVTOS

/* Define to 1 if you have the `iswprint' function. */
#undef HAVE_ISWPRINT

/* Define if IUTF8 is a defined termios mode. */
#undef HAVE_IUTF8

/* Define to 1 if you have the <langinfo.h> header file. */
#undef HAVE_LANGINFO_H

/* Define to 1 if you have the <libutil.h> header file. */
#undef HAVE_LIBUTIL_H

/* Define to 1 if you have the <limits.h> header file. */
#undef HAVE_LIMITS_H

/* Define to 1 if you have the <locale.h> header file. */
#undef HAVE_LOCALE_H

/* Define if mach_absolute_time is available. */
#undef HAVE_MACH_ABSOLUTE_TIME

/* Define to 1 if mbrtowc and mbstate_t are properly declared. */
#undef HAVE_MBRTOWC

/* Define to 1 if you have the `memchr' function. */
#undef HAVE_MEMCHR

/* Define to 1 if you have the <memory> header file. */
#define HAVE_MEMORY 1

/* Define to 1 if you have the <memory.h> header file. */
#undef HAVE_MEMORY_H

/* Define to 1 if you have the `memset' function. */
#undef HAVE_MEMSET

/* Define to 1 if the Ncurses library is present */
#undef HAVE_NCURSES

/* Define to 1 if the NcursesW library is present */
#undef HAVE_NCURSESW

/* Define to 1 if <ncursesw/curses.h> is present */
#undef HAVE_NCURSESW_CURSES_H

/* Define to 1 if <ncursesw.h> is present */
#undef HAVE_NCURSESW_H

/* Define to 1 if <ncurses/curses.h> is present */
#undef HAVE_NCURSES_CURSES_H

/* Define to 1 if <ncurses.h> is present */
#undef HAVE_NCURSES_H

/* Define to 1 if you have the <netdb.h> header file. */
#undef HAVE_NETDB_H

/* Define to 1 if you have the <netinet/in.h> header file. */
#undef HAVE_NETINET_IN_H

/* Define to 1 if you have the `nl_langinfo' function. */
#undef HAVE_NL_LANGINFO

/* Define if OSSwapHostToBigInt64 and friends exist. */
#undef HAVE_OSX_SWAP

/* Define to 1 if you have the <paths.h> header file. */
#undef HAVE_PATHS_H

/* Define if pipe2(..., O_CLOEXEC) is available. */
#undef HAVE_PIPE2_CLOEXEC

/* Define to 1 if you have the `pledge' function. */
#undef HAVE_PLEDGE

/* Define to 1 if you have the `posix_memalign' function. */
#undef HAVE_POSIX_MEMALIGN

/* Define to 1 if you have the `pselect' function. */
#define HAVE_PSELECT 1

/* Define to 1 if you have the <pty.h> header file. */
#undef HAVE_PTY_H

/* Define to 1 if you have the `setenv' function. */
#undef HAVE_SETENV

/* Define to 1 if you have the `setlocale' function. */
#undef HAVE_SETLOCALE

/* Define to 1 if you have the `setrlimit' function. */
#undef HAVE_SETRLIMIT

/* Define to 1 if you have the `sigaction' function. */
#undef HAVE_SIGACTION

/* Define to 1 if you have the `socket' function. */
#undef HAVE_SOCKET

/* Define to 1 if stdbool.h conforms to C99. */
#undef HAVE_STDBOOL_H

/* Define to 1 if you have the <stddef.h> header file. */
#undef HAVE_STDDEF_H

/* Define to 1 if you have the <stdint.h> header file. */
#undef HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
#undef HAVE_STDLIB_H

/* Define if std::shared_ptr is available. */
#define HAVE_STD_SHARED_PTR 1

/* Define if std::tr1::shared_ptr is available. */
#undef HAVE_STD_TR1_SHARED_PTR

/* Define to 1 if you have the `strchr' function. */
#undef HAVE_STRCHR

/* Define to 1 if you have the `strdup' function. */
#undef HAVE_STRDUP

/* Define to 1 if you have the `strerror' function. */
#undef HAVE_STRERROR

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#undef HAVE_STRING_H

/* Define to 1 if you have the `strncasecmp' function. */
#undef HAVE_STRNCASECMP

/* Define to 1 if you have the `strtok' function. */
#undef HAVE_STRTOK

/* Define to 1 if you have the `strtol' function. */
#undef HAVE_STRTOL

/* Define to 1 if you have the <sys/endian.h> header file. */
#undef HAVE_SYS_ENDIAN_H

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#undef HAVE_SYS_IOCTL_H

/* Define to 1 if you have the <sys/resource.h> header file. */
#undef HAVE_SYS_RESOURCE_H

/* Define to 1 if you have the <sys/socket.h> header file. */
#undef HAVE_SYS_SOCKET_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#undef HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/time.h> header file. */
#undef HAVE_SYS_TIME_H

/* Define to 1 if you have the <sys/types.h> header file. */
#undef HAVE_SYS_TYPES_H

/* Define to 1 if you have the <sys/uio.h> header file. */
#undef HAVE_SYS_UIO_H

/* Define to 1 if you have the <termios.h> header file. */
#undef HAVE_TERMIOS_H

/* Define to 1 if you have the <termio.h> header file. */
#undef HAVE_TERMIO_H

/* Define to 1 if TINFO is found */
#undef HAVE_TINFO

/* Define to 1 if you have the <tr1/memory> header file. */
#undef HAVE_TR1_MEMORY

/* Define to 1 if the system has the type `uintptr_t'. */
#undef HAVE_UINTPTR_T

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define if libutempter is available. */
#undef HAVE_UTEMPTER

/* Define to 1 if you have the <util.h> header file. */
#undef HAVE_UTIL_H

/* Define to 1 if you have the <utmpx.h> header file. */
#undef HAVE_UTMPX_H

/* Define to 1 if you have the `vfork' function. */
#undef HAVE_VFORK

/* Define to 1 if you have the <vfork.h> header file. */
#undef HAVE_VFORK_H

/* Define to 1 if you have the <wchar.h> header file. */
#undef HAVE_WCHAR_H

/* Define to 1 if you have the <wctype.h> header file. */
#undef HAVE_WCTYPE_H

/* Define to 1 if you have the `wcwidth' function. */
#undef HAVE_WCWIDTH

/* Define to 1 if `fork' works. */
#undef HAVE_WORKING_FORK

/* Define to 1 if `vfork' works. */
#undef HAVE_WORKING_VFORK

/* Define to 1 if the system has the type `_Bool'. */
#undef HAVE__BOOL

/* Name of package */
#undef PACKAGE

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the home page for this package. */
#undef PACKAGE_URL

/* Define to the version of this package. */
#undef PACKAGE_VERSION

/* Define to 1 if you have the ANSI C header files. */
#undef STDC_HEADERS

/* Use Apple Common Crypto library */
#undef USE_APPLE_COMMON_CRYPTO_AES

/* Use Nettle library */
#undef USE_NETTLE_AES

/* Use OpenSSL library */
// #define USE_OPENSSL_AES 0
#define USE_REFERENCE_AES 1

// #undef FULL_UNROLL

/* Version number of package */
#undef VERSION

/* Define for Solaris 2.5.1 so the uint32_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
#undef _UINT32_T

/* Define for Solaris 2.5.1 so the uint64_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
#undef _UINT64_T

/* Define for Solaris 2.5.1 so the uint8_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
#undef _UINT8_T

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#undef inline
#endif

/* Define to the type of a signed integer type of width exactly 64 bits if
   such a type exists and the standard includes do not define it. */
#undef int64_t

/* Define to `int' if <sys/types.h> does not define. */
#undef pid_t

/* Define to the equivalent of the C99 'restrict' keyword, or to
   nothing if this is not supported.  Do not define if restrict is
   supported directly.  */
#undef restrict
/* Work around a bug in Sun C++: it does not support _Restrict or
   __restrict__, even though the corresponding Sun C compiler ends up with
   "#define restrict _Restrict" or "#define restrict __restrict__" in the
   previous line.  Perhaps some future version of Sun C++ will work with
   restrict; if so, hopefully it defines __RESTRICT like Sun C does.  */
#if defined __SUNPRO_CC && !defined __RESTRICT
# define _Restrict
# define __restrict__
#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
#undef size_t

/* Define to `int' if <sys/types.h> does not define. */
#undef ssize_t

/* Define to the type of an unsigned integer type of width exactly 16 bits if
   such a type exists and the standard includes do not define it. */
#undef uint16_t

/* Define to the type of an unsigned integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
#undef uint32_t

/* Define to the type of an unsigned integer type of width exactly 64 bits if
   such a type exists and the standard includes do not define it. */
#undef uint64_t

/* Define to the type of an unsigned integer type of width exactly 8 bits if
   such a type exists and the standard includes do not define it. */
#undef uint8_t

/* Define to the type of an unsigned integer type wide enough to hold a
   pointer, if such a type exists, and if the system does not define it. */
#undef uintptr_t

/* Define as `fork' if `vfork' does not work. */
#undef vfork

/* -------------------------------------------------------------------------------------- */

#define GOOGLE_PROTOBUF_NO_RTTI
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* the name of <hash_map> */
#define HASH_MAP_CLASS unordered_map

/* the location of <unordered_map> or <hash_map> */
#define HASH_MAP_H <tr1/unordered_map>

/* the namespace of hash_map/hash_set */
#define HASH_NAMESPACE std::tr1

/* the name of <hash_set> */
#define HASH_SET_CLASS unordered_set

/* the location of <unordered_set> or <hash_set> */
#define HASH_SET_H <tr1/unordered_set>

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `ftruncate' function. */
#define HAVE_FTRUNCATE 1

/* define if the compiler has hash_map */
#define HAVE_HASH_MAP 1

/* define if the compiler has hash_set */
#define HAVE_HASH_SET 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the `mkdir' function. */
#define HAVE_MKDIR 1

/* Define if you have POSIX threads libraries and header files. */
#define HAVE_PTHREAD 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strchr' function. */
#define HAVE_STRCHR 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strtol' function. */
#define HAVE_STRTOL 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Enable classes using zlib compression. */
#define HAVE_ZLIB 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "protobuf"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "protobuf@googlegroups.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "Protocol Buffers"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "Protocol Buffers 3.1.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "protobuf"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "3.1.0"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Version number of package */
#define VERSION "3.1.0"

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */
