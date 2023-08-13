macro(build_script_header)
    message(STATUS "Script starting: ${CMAKE_CURRENT_LIST_FILE}")

    set(BUILD_SCRIPTS_DIR "${CMAKE_CURRENT_LIST_DIR}")
    set(BUILD_TOP_DIR "${BUILD_SCRIPTS_DIR}/..")
    set(PROJECT_TOP_DIR "${BUILD_TOP_DIR}/..")
endmacro()


macro(build_script_footer)
    execute_process(COMMAND "${CMAKE_COMMAND}"
                            -D "PROJECT_TOP_DIR:PATH=${PROJECT_TOP_DIR}"
                            -D "BUILD_TOP_DIR:PATH=${BUILD_TOP_DIR}"
                            -D "TOOLCHAIN:STRING=${TOOLCHAIN}"
                            -D "GENERATOR:STRING=${GENERATOR}"
                            -D "BUILD_CONFIG:STRING=${BUILD_CONFIG}"
                            -D "BUILD_PROJECT_DIR_POSTFIX:STRING=${BUILD_PROJECT_DIR_POSTFIX}"
                            -D "BUILD_PROJECT_DIR_SUBPATH:STRING=${BUILD_PROJECT_DIR_SUBPATH}"
                            -P "${PROJECT_TOP_DIR}/cmake/scripts/make_project.cmake"
                    # COMMAND_ECHO STDOUT
                    # OUTPUT_FILE "${CMAKE_CURRENT_LIST_FILE}.configure.log"
                    # ERROR_FILE "${CMAKE_CURRENT_LIST_FILE}.configure.log"
                    )

    message(STATUS "Script finished: ${CMAKE_CURRENT_LIST_FILE}")
endmacro()


macro(build_config_define)
    if(NOT BUILD_CONFIG)
        set(BUILD_CONFIG "Debug")
    endif()
    string(TOLOWER "${BUILD_CONFIG}" BUILD_CONFIG_LOWERED)
    set(BUILD_PROJECT_DIR_SUBPATH "${BUILD_CONFIG_LOWERED}")
endmacro()
