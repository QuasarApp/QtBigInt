#
# Copyright (C) 2018-2021 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

cmake_minimum_required(VERSION 3.10)
project(QtBigint LANGUAGES CXX)
if(TARGET ${PROJECT_NAME})
  message("The ${PROJECT_NAME} arledy included in main Project")
  return()
endif()

include(CMake/QuasarApp.cmake)
include(CMake/Version.cmake)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Core QUIET)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core QUIET)

if (NOT DEFINED BIGINT_TESTS)
    set(BIGINT_TESTS ON)

    if (DEFINED TARGET_PLATFORM_TOOLCHAIN)
        if (${TARGET_PLATFORM_TOOLCHAIN} STREQUAL "wasm32")
            set(BIGINT_TESTS OFF)
        endif()
    endif()

    if (ANDROID OR IOS)
        set(BIGINT_TESTS OFF)
    endif()

    if (NOT QT_VERSION_MAJOR)
        set(BIGINT_TESTS OFF)
    endif()
endif()


add_subdirectory(src)

if (BIGINT_TESTS)
    add_subdirectory(tests)
else()
    message("The ${PROJECT_NAME} tests is disabled.")
endif()


initAll()

addDoc(${PROJECT_NAME}Docs ${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf)

