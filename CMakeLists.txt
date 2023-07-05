cmake_minimum_required(VERSION 3.22)

set(USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(immortalsssl)

add_definitions(-DDATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Data")

if(MSVC)
  set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
  foreach(CompilerFlag ${CompilerFlags})
    set(${CompilerFlag} "${${CompilerFlag}} /MP")
  endforeach()
  message(STATUS "Added parallel build arguments to CMAKE_*_FLAGS")
elseif(${CMAKE_GENERATOR} MATCHES "Unix Makefiles")
	message(STATUS ${CMAKE_BUILD_TOOL})
	include(ProcessorCount)
	ProcessorCount(CORES_COUNT)
	if(CORES_COUNT EQUAL 0)
		set(CORES_COUNT 4)
		message(STATUS "Failed to determine cpu cores, using 4")
	else()
		message(STATUS "Found ${CORES_COUNT} cpu cores")
	endif()
    set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j${CORES_COUNT}")
    message(STATUS "Added parallel build arguments to CMAKE_MAKE_PROGRAM")
endif()

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

file(GLOB_RECURSE GEN_PROTO_DEF "Source/*.proto")

set(PROTO_SRCS)
set(PROTO_HDRS)

foreach(file ${GEN_PROTO_DEF})
    get_filename_component(FILE_ABS ${file} ABSOLUTE)
    get_filename_component(FILE_WE ${file} NAME_WE)
    get_filename_component(FILE_PATH ${file} DIRECTORY)

    set(GEN_FILE_HEADER "${FILE_PATH}/${FILE_WE}.pb.h")
    set(GEN_FILE_SOURCE "${FILE_PATH}/${FILE_WE}.pb.cc")

    file(RELATIVE_PATH GEN_FILE_HEADER_REL ${CMAKE_CURRENT_SOURCE_DIR} ${GEN_FILE_HEADER})
    file(RELATIVE_PATH GEN_FILE_SOURCE_REL ${CMAKE_CURRENT_SOURCE_DIR} ${GEN_FILE_SOURCE})

    list(APPEND PROTO_SRCS ${GEN_FILE_SOURCE_REL})
    list(APPEND PROTO_HDRS ${GEN_FILE_HEADER_REL})

    add_custom_command(
      OUTPUT ${GEN_FILE_SOURCE}
             ${GEN_FILE_HEADER}
      COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
      ARGS --proto_path=${FILE_PATH} --cpp_out=${FILE_PATH} ${FILE_ABS}
      DEPENDS ${FILE_ABS}
      COMMENT "Running C++ protocol buffer compiler on ${file}"
      VERBATIM )
endforeach()

file(GLOB_RECURSE GEN_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Source/*.c" "Source/*.cpp" "/Source/*.cc" "/Source/*.cxx")
file(GLOB_RECURSE GEN_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Source/*.h" "Source/*.hpp")

set(SOURCE_FILES
    ${GEN_HEADER_FILES}
    ${PROTO_HDRS}
    ${GEN_SOURCE_FILES}
    ${PROTO_SRCS})

add_executable(immortalsssl ${SOURCE_FILES})

foreach(FILE ${SOURCE_FILES})
  get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

  string(REPLACE "/" "\\\\" GROUP "${PARENT_DIR}")

  source_group(${GROUP} FILES ${FILE})
endforeach()

if(WIN32)
  target_link_libraries(immortalsssl wsock32)
endif()
target_link_libraries(immortalsssl ${PROTOBUF_LIBRARIES} ${ZeroMQ_LIBRARIES})