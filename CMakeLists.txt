# Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# cmake build file for C++ helloworld example.
# Assumes protobuf and gRPC have been installed using cmake.
# See cmake_externalproject/CMakeLists.txt for all-in-one cmake build
# that automatically builds all the dependencies before building helloworld.

cmake_minimum_required(VERSION 3.5.1)

project(CPP-CLUE)

include(./common.cmake)

# Proto file
get_filename_component(clue_proto "clue.proto" ABSOLUTE)
get_filename_component(clue_proto_path "${clue_proto}" PATH)

# Generated sources
set(clue_proto_srcs "clue.pb.cc")
set(clue_proto_hdrs "clue.pb.h")
set(clue_grpc_srcs "clue.grpc.pb.cc")
set(clue_grpc_hdrs "clue.grpc.pb.h")
add_custom_command(
      OUTPUT "${clue_proto_srcs}" "${clue_proto_hdrs}" "${clue_grpc_srcs}" "${clue_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${clue_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${clue_proto}"
      DEPENDS "${clue_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# clue_grpc_proto
add_library(clue_grpc_proto
  ${clue_grpc_srcs}
  ${clue_grpc_hdrs}
  ${clue_proto_srcs}
  ${clue_proto_hdrs})
target_link_libraries(clue_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# clue client
file(GLOB SOURCES "./src/*.cc")

# Targets
foreach(_target
  "clue"
  test_client)
  add_executable(${_target}
    ${SOURCES})
  target_link_libraries(${_target}
    clue_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()
