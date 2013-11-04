# Install script for directory: /Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include/fann.h"
    "/Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include/doublefann.h"
    "/Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include/fann_internal.h"
    "/Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include/floatfann.h"
    "/Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include/fann_data.h"
    "/Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include/fixedfann.h"
    "/Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include/compat_time.h"
    "/Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include/fann_activation.h"
    "/Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include/fann_cascade.h"
    "/Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include/fann_error.h"
    "/Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include/fann_train.h"
    "/Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include/fann_io.h"
    "/Users/sadeqsalehi/Desktop/FANN-2.2.0-Source/src/include/fann_cpp.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

