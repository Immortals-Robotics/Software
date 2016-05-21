# CMake helper macros
# Ali Salehi, Spring 2016

macro(enable_cpp11_support)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
      message(STATUS "Adding -std=c++11")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  endif()
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
      message(STATUS "Adding -std=c11")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
  endif()
endmacro()

macro(enable_parallel_build)
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
endmacro()

macro(set_files_group SOURCE_FILES)
  foreach(FILE ${${SOURCE_FILES}})
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    string(REPLACE "/" "\\\\" GROUP "${PARENT_DIR}")

    source_group(${GROUP} FILES ${FILE})
  endforeach()
endmacro()

macro(set_project_paths)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")
  add_definitions(-DDATA_PATH="${CMAKE_SOURCE_DIR}/Data")
endmacro()

macro(add_protobuf_generate_command PROTO_DEF PROTO_SRCS PROTO_HDRS)
  set(${PROTO_SRCS})
  set(${PROTO_HDRS})
  foreach(file ${${PROTO_DEF}})
      get_filename_component(FILE_ABS ${file} ABSOLUTE)
      get_filename_component(FILE_WE ${file} NAME_WE)
      get_filename_component(FILE_PATH ${file} DIRECTORY)

      set(GEN_FILE_HEADER "${FILE_PATH}/${FILE_WE}.pb.h")
      set(GEN_FILE_SOURCE "${FILE_PATH}/${FILE_WE}.pb.cc")

      file(RELATIVE_PATH GEN_FILE_HEADER_REL ${CMAKE_CURRENT_SOURCE_DIR} ${GEN_FILE_HEADER})
      file(RELATIVE_PATH GEN_FILE_SOURCE_REL ${CMAKE_CURRENT_SOURCE_DIR} ${GEN_FILE_SOURCE})

      list(APPEND ${PROTO_SRCS} ${GEN_FILE_SOURCE_REL})
      list(APPEND ${PROTO_HDRS} ${GEN_FILE_HEADER_REL})

      add_custom_command(
        OUTPUT ${GEN_FILE_SOURCE}
               ${GEN_FILE_HEADER}
        COMMAND  ${PROTOBUF_PROTOC_EXECUTABLE}
        ARGS --proto_path=${FILE_PATH} --cpp_out=${FILE_PATH} ${FILE_ABS}
        DEPENDS ${FILE_ABS}
        COMMENT "Running C++ protocol buffer compiler on ${file}"
        VERBATIM )
  endforeach()
endmacro()