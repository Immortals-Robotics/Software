cmake_minimum_required(VERSION 3.0)
project(${CMAKE_PROJECT_NAME}-reality)

include("${CMAKE_SOURCE_DIR}/CMake/Helpers.cmake")

set_project_paths()
enable_parallel_build()
enable_cpp11_support()

find_package(ZeroMQ REQUIRED)
include_directories(${ZeroMQ_INCLUDE_DIR})

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

file(GLOB_RECURSE GEN_PROTO_DEF "*.proto")

add_protobuf_generate_command(GEN_PROTO_DEF PROTO_SRCS PROTO_HDRS)

file(GLOB_RECURSE GEN_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.c" "*.cpp" "*.cc" "*.cxx")
file(GLOB_RECURSE GEN_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" "*.hpp")

set(SOURCE_FILES
    ${GEN_HEADER_FILES}
    ${PROTO_HDRS}
    ${GEN_SOURCE_FILES}
    ${PROTO_SRCS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set_files_group(SOURCE_FILES)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} wsock32)
endif()
target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARIES} ${ZeroMQ_LIBRARIES})