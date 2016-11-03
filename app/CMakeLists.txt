# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

set(CMAKE_CXX_CREATE_SHARED_LIBRARY "${CMAKE_CXX_CREATE_SHARED_LIBRARY} -lz")

# cmake -DPLATFORM:STRING=pc
SET(PLATFORM "android" CACHE STRING "Target platform")

include_directories(src/main/cpp/config/)
include_directories(src/main/cpp/libmosh/src/crypto/)
include_directories(src/main/cpp/libmosh/src/util/)
include_directories(src/main/cpp/main/)
include_directories(src/main/cpp/libmosh/src/network/)
include_directories(src/main/cpp/statesync/)
include_directories(src/main/cpp/protobuf-generated/)
include_directories(src/main/cpp/libprotobuf/)
include_directories(src/main/cpp/libprotobuf/google/protobuf/)

set(libprotobuf_lite_files
  src/main/cpp/libprotobuf/google/protobuf/arena.cc
  src/main/cpp/libprotobuf/google/protobuf/arenastring.cc
  src/main/cpp/libprotobuf/google/protobuf/extension_set.cc
  src/main/cpp/libprotobuf/google/protobuf/generated_message_util.cc
  src/main/cpp/libprotobuf/google/protobuf/io/coded_stream.cc
  src/main/cpp/libprotobuf/google/protobuf/io/zero_copy_stream.cc
  src/main/cpp/libprotobuf/google/protobuf/io/zero_copy_stream_impl_lite.cc
  src/main/cpp/libprotobuf/google/protobuf/message_lite.cc
  src/main/cpp/libprotobuf/google/protobuf/repeated_field.cc
  src/main/cpp/libprotobuf/google/protobuf/stubs/atomicops_internals_x86_gcc.cc
  src/main/cpp/libprotobuf/google/protobuf/stubs/atomicops_internals_x86_msvc.cc
  src/main/cpp/libprotobuf/google/protobuf/stubs/bytestream.cc
  src/main/cpp/libprotobuf/google/protobuf/stubs/common.cc
  src/main/cpp/libprotobuf/google/protobuf/stubs/int128.cc
  src/main/cpp/libprotobuf/google/protobuf/stubs/once.cc
  src/main/cpp/libprotobuf/google/protobuf/stubs/status.cc
  src/main/cpp/libprotobuf/google/protobuf/stubs/statusor.cc
  src/main/cpp/libprotobuf/google/protobuf/stubs/stringpiece.cc
  src/main/cpp/libprotobuf/google/protobuf/stubs/stringprintf.cc
  src/main/cpp/libprotobuf/google/protobuf/stubs/structurally_valid.cc
  src/main/cpp/libprotobuf/google/protobuf/stubs/strutil.cc
  src/main/cpp/libprotobuf/google/protobuf/stubs/time.cc
  src/main/cpp/libprotobuf/google/protobuf/wire_format_lite.cc
)

#~/Downloads/protoc-3.1.0-linux-x86_64.exe --proto_path=cpp --cpp_out=cpp/protobuf-generated cpp/protobufs/hostinput.proto cpp/protobufs/transportinstruction.proto cpp/protobufs/userinput.proto

add_library(protobuf SHARED ${libprotobuf_lite_files})

add_library(mycrypto SHARED
              src/main/cpp/libmosh/src/crypto/base64.cc
              src/main/cpp/libmosh/src/crypto/crypto.cc
              src/main/cpp/libmosh/src/crypto/rijndael-alg-fst.cc
              src/main/cpp/libmosh/src/crypto/ocb.cc)

add_library(protobuf-generated SHARED
              src/main/cpp/protobuf-generated/protobufs/hostinput.pb.cc
              src/main/cpp/protobuf-generated/protobufs/transportinstruction.pb.cc
              src/main/cpp/protobuf-generated/protobufs/userinput.pb.cc)

add_library(util SHARED
              src/main/cpp/libmosh/src/util/swrite.cc
              src/main/cpp/libmosh/src/util/timestamp.cc)

add_library(mynetwork SHARED
              src/main/cpp/libmosh/src/network/compressor.cc
              src/main/cpp/libmosh/src/network/network.cc
              src/main/cpp/libmosh/src/network/networktransport-impl.h
              src/main/cpp/libmosh/src/network/transportfragment.cc
              src/main/cpp/libmosh/src/network/transportsender-impl.h)

add_library(statesync SHARED
              src/main/cpp/statesync/user.cc)

target_link_libraries( protobuf-generated
                       protobuf
                       z )

target_link_libraries( mycrypto
                       util )

target_link_libraries( mynetwork
                       mycrypto
#                      util
                       protobuf-generated
#                      protobuf
                       z )

target_link_libraries( statesync
                       mynetwork )

if (${PLATFORM} STREQUAL "android")
  add_library( iotkbd-core SHARED
             src/main/cpp/main/main.cc
             src/main/cpp/main/select.cc )

  find_library( log-lib log )

  target_link_libraries( util
                         ${log-lib} )

  target_link_libraries( iotkbd-core
                         protobuf
                         protobuf-generated
                         mycrypto
                         mynetwork
                         statesync
                         util
                         z
                         ${log-lib} )
endif()

# create some directory like native-build
# cmake -DPLATFORM:STRING=pc ../app/
# cmake --build .
if (${PLATFORM} STREQUAL "pc")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  add_executable(IODkbdServer
                   src/main/cpp/main/server.cc
                   src/main/cpp/main/select.cc )
  target_link_libraries(IODkbdServer protobuf
                           protobuf-generated
                           mycrypto
                           mynetwork
                           statesync
                           util
                           z )
endif()
