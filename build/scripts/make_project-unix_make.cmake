#Usage: cmake [-DBUILD_CONFIG=<Debug|Release|RelWithDebInfo|MinSizeRel>] -P make_project-unix-make.cmake

cmake_minimum_required(VERSION 3.13)

if (NOT CMAKE_SCRIPT_MODE_FILE OR CMAKE_PARENT_LIST_FILE)
    message(FATAL_ERROR "Script file ${CMAKE_CURRENT_LIST_FILE} should not be included")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/build-script-macros.cmake)

build_script_header()
build_project_postfix_define(DEFAULT "")
build_config_define()

set(TOOLCHAIN "host")
set(GENERATOR "Unix Makefiles")

build_script_footer()
