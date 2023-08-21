#Usage: cmake [-DBUILD_PROJECT_POSTFIX=<SOME_POSTFIX>] -P make_project-vs17.cmake

cmake_minimum_required(VERSION 3.13)

if (NOT CMAKE_SCRIPT_MODE_FILE OR CMAKE_PARENT_LIST_FILE)
    message(FATAL_ERROR "Script file ${CMAKE_CURRENT_LIST_FILE} should not be included")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/build-script-macros.cmake)

build_script_header()
build_project_postfix_define(DEFAULT "")

set(TOOLCHAIN "host")
set(GENERATOR "Visual Studio 17 2022")

build_script_footer()
