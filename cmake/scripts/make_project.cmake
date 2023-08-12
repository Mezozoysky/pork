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

if(NOT GENERATOR)
    set(GENERATOR "Ninja Multi-Config")
endif()
message(STATUS "GENERATOR: '${GENERATOR}'")

string(TOLOWER "${TOOLCHAIN}" TOOLCHAIN)
if("${TOOLCHAIN}" STREQUAL "auto" OR "${TOOLCHAIN}" STREQUAL "host")
    set(TOOLCHAIN "")
endif()

list(PREPEND CMAKE_MODULE_PATH "${PROJECT_TOP_DIR}/cmake/pork-cmake-modules")
include("pork-platform-macros")

init_host_system_vars()
message(STATUS "PORK_HOST_SYSTEM: '${PORK_HOST_SYSTEM}'")
message(STATUS "PORK_HOST_PLATFORM: '${PORK_HOST_PLATFORM}'")
message(STATUS "PORK_HOST_BITNESS: ${PORK_HOST_BITNESS}")

set(TOOLCHAIN_FILE)
if("${TOOLCHAIN}" STREQUAL "")
    # set(PORK_SYSTEM "${PORK_HOST_SYSTEM}")
    # set(PORK_PLATFORM "${PORK_HOST_PLATFORM}")
    # set(PORK_BITNESS "${PORK_HOST_BITNESS}")
else()
    set(TOOLCHAIN_FILE "${PROJECT_TOP_DIR}/cmake/toolchains/${TOOLCHAIN}.cmake")
    if(NOT EXISTS "${TOOLCHAIN_FILE}")
        message(FATAL_ERROR "Toolchain doesn't exist: '${TOOLCHAIN}'")
    endif()
    include("${TOOLCHAIN_FILE}")
endif()
message(STATUS "PORK_SYSTEM: '${PORK_SYSTEM}'")
message(STATUS "PORK_PLATFORM: '${PORK_PLATFORM}'")
message(STATUS "PORK_BITNESS: ${PORK_BITNESS}")
set(BUILD_PROJECT_DIR "${BUILD_TOP_DIR}/${PORK_SYSTEM}-${PORK_PLATFORM}")
if(NOT "${BUILD_PROJECT_DIR_POSTFIX}" STREQUAL "")
  set(BUILD_PROJECT_DIR "${BUILD_PROJECT_DIR}-${BUILD_PROJECT_DIR_POSTFIX}")
endif()
message(STATUS "BUILD_PROJECT_DIR: '${BUILD_PROJECT_DIR}'")

message(STATUS "-------- Start project generation --------")
execute_process(COMMAND "${CMAKE_COMMAND}"
                        -S "${PROJECT_TOP_DIR}"
                        -B "${BUILD_PROJECT_DIR}"
                        -D "CMAKE_TOOLCHAIN_FILE:PATH=${TOOLCHAIN_FILE}"
                        -D "CMAKE_BUILD_TYPE:STRING=${BUILD_CONFIG}"
                )
message(STATUS "-------- Project generation finished --------")

message(STATUS "Script finished: ${CMAKE_CURRENT_LIST_FILE}")

