cmake_minimum_required(VERSION 3.0)
project(${CMAKE_PROJECT_NAME}-reality)

include("${CMAKE_SOURCE_DIR}/CMake/Helpers.cmake")

set_project_paths()
enable_parallel_build()
enable_cpp11_support()

if(MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
endif()

file(GLOB_RECURSE GEN_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.c" "*.cpp" "*.cc" "*.cxx")
file(GLOB_RECURSE GEN_HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" "*.hpp")

set(SOURCE_FILES
    ${GEN_HEADER_FILES}
    ${GEN_SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set_files_group(SOURCE_FILES)

target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}-common)