cmake_minimum_required(VERSION 3.13)

if (NOT CMAKE_SCRIPT_MODE_FILE OR CMAKE_PARENT_LIST_FILE)
    message(FATAL_ERROR "Script file ${CMAKE_CURRENT_LIST_FILE} should not be included")
endif()

message(STATUS "Script starting: ${CMAKE_CURRENT_LIST_FILE}")

if(NOT PROJECT_TOP_DIR)
    set(PROJECT_TOP_DIR ${CMAKE_CURRENT_LIST_DIR})
    message(STATUS "PROJECT_TOP_DIR isn't set so using '${PROJECT_TOP_DIR}' as PROJECT_TOP_DIR")
else()
    message(STATUS "PROJECT_TOP_DIR: '${PROJECT_TOP_DIR}'")
endif()

if(NOT BUILD_TOP_DIR)
    set(BUILD_TOP_DIR "${PROJECT_TOP_DIR}/build")
    message(STATUS "BUILD_TOP_DIR isn't set so using '${BUILD_TOP_DIR}' as BUILD_TOP_DIR")
else()
  message(STATUS "BUILD_TOP_DIR: '${BUILD_TOP_DIR}'")
endif()

string(TOLOWER "${TOOLCHAIN}" TOOLCHAIN)
if("${TOOLCHAIN}" MATCHES "^$" OR "${TOOLCHAIN}" MATCHES "^auto$" OR "${TOOLCHAIN}" MATCHES "^host$")
    set(TOOLCHAIN)
endif()

list(PREPEND CMAKE_MODULE_PATH "${PROJECT_TOP_DIR}/cmake/pork-cmake-modules")
include("pork-platform-macros")

init_host_system_vars()
message(STATUS "PORK_HOST_SYSTEM: '${PORK_HOST_SYSTEM}'")
message(STATUS "PORK_HOST_PLATFORM: '${PORK_HOST_PLATFORM}'")
message(STATUS "PORK_HOST_BITNESS: ${PORK_HOST_BITNESS}")

set(TOOLCHAIN_FILE)
if(NOT TOOLCHAIN)
    # set(PORK_SYSTEM "${PORK_HOST_SYSTEM}")
    # set(PORK_PLATFORM "${PORK_HOST_PLATFORM}")
    # set(PORK_BITNESS "${PORK_HOST_BITNESS}")
else()
    if(TARGET_ID MATCHES "mingw-x86")
        set(TOOLCHAIN_FILE "${PROJECT_TOP_DIR}/cmake/toolchains/mingw-x86.cmake")
    elseif(TARGET_ID MATCHES "mingw-x86_64")
        set(TOOLCHAIN_FILE "${PROJECT_TOP_DIR}/cmake/toolchains/mingw-x86_64.cmake")
    endif()
    include("${TOOLCHAIN_FILE}")
endif()
message(STATUS "PORK_SYSTEM: '${PORK_SYSTEM}'")
message(STATUS "PORK_PLATFORM: '${PORK_PLATFORM}'")
message(STATUS "PORK_BITNESS: ${PORK_BITNESS}")
set(BUILD_PROJECT_DIR "${BUILD_TOP_DIR}/${PORK_SYSTEM}-${PORK_PLATFORM}")
message(STATUS "BUILD_PROJECT_DIR: '${BUILD_PROJECT_DIR}'")

message(STATUS "-------- Start project generation --------")
execute_process(COMMAND "${CMAKE_COMMAND}" "-S${PROJECT_TOP_DIR}" "-B${BUILD_PROJECT_DIR}" "-DCMAKE_TOOLCHAIN_FILE:PATH=${TOOLCHAIN_FILE}")
message(STATUS "-------- Project generation finished --------")

message(STATUS "Script finished: ${CMAKE_CURRENT_LIST_FILE}")

