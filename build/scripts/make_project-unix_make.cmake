#Usage: cmake [-DBUILD_CONFIG=<Debug|Release|RelWithDebInfo|MinSizeRel>] -P make_project-unix-make.cmake

cmake_minimum_required(VERSION 3.13)

if (NOT CMAKE_SCRIPT_MODE_FILE OR CMAKE_PARENT_LIST_FILE)
    message(FATAL_ERROR "Script file ${CMAKE_CURRENT_LIST_FILE} should not be included")
endif()

message(STATUS "Script starting: ${CMAKE_CURRENT_LIST_FILE}")

if(NOT BUILD_CONFIG)
    set(BUILD_CONFIG "Release")
endif()
string(TOLOWER "${BUILD_CONFIG}" BUILD_PROJECT_DIR_POSTFIX)

set(BUILD_SCRIPTS_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(BUILD_TOP_DIR "${BUILD_SCRIPTS_DIR}/..")
set(PROJECT_TOP_DIR "${BUILD_TOP_DIR}/..")

set(GENERATOR "Unix Makefiles")
set(TOOLCHAIN "host")

execute_process(COMMAND "${CMAKE_COMMAND}"
                        -D "PROJECT_TOP_DIR:PATH=${PROJECT_TOP_DIR}"
                        -D "BUILD_TOP_DIR:PATH=${BUILD_TOP_DIR}"
                        -D "TOOLCHAIN:STRING=${TOOLCHAIN}"
                        -D "GENERATOR:STRING=${GENERATOR}"
                        -D "BUILD_CONFIG:STRING=${BUILD_CONFIG}"
                        -D "BUILD_PROJECT_DIR_POSTFIX:STRING=${BUILD_PROJECT_DIR_POSTFIX}"
                        -P "${PROJECT_TOP_DIR}/cmake/scripts/make_project.cmake"
                # COMMAND_ECHO STDOUT
                # OUTPUT_FILE "${CMAKE_CURRENT_LIST_FILE}.configure.log"
                # ERROR_FILE "${CMAKE_CURRENT_LIST_FILE}.configure.log"
                )

message(STATUS "Script finished: ${CMAKE_CURRENT_LIST_FILE}")

