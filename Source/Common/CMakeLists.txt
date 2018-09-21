cmake_minimum_required(VERSION 3.0)
project(${CMAKE_PROJECT_NAME}-common)

include("${CMAKE_SOURCE_DIR}/CMake/Helpers.cmake")

set_project_paths()
enable_parallel_build()
enable_cpp11_support()

if(MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")

	add_definitions(-DWIN32_LEAN_AND_MEAN)
	add_definitions(-DNOMINMAX)
endif()

if(MSVC)
	set(PROTOBUF_SRC_ROOT_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/protobuf")
	set(ZeroMQ_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zeromq")  
endif()

find_package(ZeroMQ REQUIRED)
include_directories(${ZeroMQ_INCLUDE_DIR})

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

include(${CMAKE_CURRENT_SOURCE_DIR}/protos/CMakeLists.txt.inc)
set(GEN_PROTO_DEF "")
foreach(f ${GEN_PROTO_DEF_FILES})
    list(APPEND GEN_PROTO_DEF "${CMAKE_CURRENT_SOURCE_DIR}/protos/${f}")
endforeach(f)

add_protobuf_generate_command(GEN_PROTO_DEF PROTO_SRCS PROTO_HDRS)

file(GLOB_RECURSE GEN_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.c" "*.cpp" "*.cc" "*.cxx")
file(GLOB_RECURSE GEN_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" "*.hpp")

set(SOURCE_FILES
    ${GEN_HEADER_FILES}
    ${PROTO_HDRS}
    ${GEN_SOURCE_FILES}
    ${PROTO_SRCS})

add_library(${PROJECT_NAME} ${SOURCE_FILES})

set_files_group(SOURCE_FILES)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} INTERFACE ${ZeroMQ_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} INTERFACE ${PROTOBUF_INCLUDE_DIRS})

if(WIN32)
  target_link_libraries(${PROJECT_NAME} wsock32 ws2_32 iphlpapi)
  target_compile_definitions(${PROJECT_NAME} PUBLIC ZMQ_STATIC)
endif()
target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARIES} ${ZeroMQ_LIBRARIES})