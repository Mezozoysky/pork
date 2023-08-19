cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

include(CMakeParseArguments)
include(GNUInstallDirs)


macro(init_build_types)
    cmake_parse_arguments(ARGS # prefix
        "" # list of options (boolean args)
        "DEFAULT" # list of single value args
        "AVAILABLE" # list of multi value args
        ${ARGN}
        )

    if (GENERATOR_MULTI)
        if (NOT CMAKE_CONFIGURATION_TYPES)
            if(NOT ARGS_AVAILABLE)
                set(ARGS_AVAILABLE "Debug" "Release" "RelWithDebInfo")
            endif()
            set(CMAKE_CONFIGURATION_TYPES ${ARGS_AVAILABLE})
        endif()
        message(STATUS "build configurations: ${CMAKE_CONFIGURATION_TYPES}")
    else()
        if (NOT CMAKE_BUILD_TYPE)
            if(NOT ARGS_DEFAULT)
                set(ARGS_DEFAULT "Debug")
            endif()
            set(CMAKE_BUILD_TYPE "${ARGS_DEFAULT}")
        endif()
        set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Build type" FORCE)
        set_property(CACHE CMAKE_BUILD_TYPE
                     PROPERTY STRINGS ${ARGS_AVAILABLE})
        message(STATUS "build type: ${CMAKE_BUILD_TYPE}")
    endif()
endmacro()


macro(init_output_directories)
    cmake_parse_arguments(ARGS # prefix
        "NO_GNU_DIRS;LOWER_DIR_NAMES" # list of options (boolean args)
        "OUTPUT_PREFIX" # list of single value args
        "" # list of multi value args
        ${ARGN}
        )

    set(output_dir_prefix "${CMAKE_BINARY_DIR}")
    if(ARGS_OUTPUT_PREFIX)
        set(output_dir_prefix "${ARGS_OUTPUT_PREFIX}")
    endif()

    set(bt_list ${CMAKE_CONFIGURATION_TYPES})
    if (NOT bt_list)
        set(bt_list "${CMAKE_BUILD_TYPE};")
    endif()
    foreach(bt ${bt_list})
        set(bt_upper)
        string(TOUPPER ${bt} bt_upper)
        set(bt_dir_name "${bt}")
        if(ARGS_LOWER_DIR_NAMES)
            string(TOLOWER ${bt_dir_name} bt_dir_name)
        endif()
        set(archive_var_name "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${bt_upper}")
        set(library_var_name "CMAKE_LIBRARY_OUTPUT_DIRECTORY_${bt_upper}")
        set(runtime_var_name "CMAKE_RUNTIME_OUTPUT_DIRECTORY_${bt_upper}")

        # archive dir
        if(NOT ${archive_var_name})
            set(archive_dir "${output_dir_prefix}")
            if(GENERATOR_MULTI)
                set(archive_dir "${archive_dir}/${bt_dir_name}")
            endif()
            if(NOT ARGS_NO_GNU_DIR)
                set(archive_dir "${archive_dir}/${CMAKE_INSTALL_LIBDIR}")
            endif()
            set(${archive_var_name} "${archive_dir}")
        else()
        endif()
        set(${archive_var_name} "${${archive_var_name}}" CACHE PATH "Output directory for static libraries." FORCE)

        # library dir
        if(NOT ${library_var_name})
            set(library_dir "${output_dir_prefix}")
            if(GENERATOR_MULTI)
                set(library_dir "${library_dir}/${bt_dir_name}")
            endif()
            if(NOT ARGS_NO_GNU_DIR)
              set(library_dir "${library_dir}/${CMAKE_INSTALL_LIBDIR}")
            endif()
            set(${library_var_name} "${library_dir}")
        else()
        endif()
        set(${library_var_name} "${${library_var_name}}" CACHE PATH "Output directory for shared libraries." FORCE)

        # runtime dir
        if(NOT ${runtime_var_name})
            set(runtime_dir "${output_dir_prefix}")
            if(GENERATOR_MULTI)
                set(runtime_dir "${runtime_dir}/${bt_dir_name}")
            endif()
            if(NOT ARGS_NO_GNU_DIR)
              set(runtime_dir "${runtime_dir}/${CMAKE_INSTALL_BINDIR}")
            endif()
            set(${runtime_var_name} "${runtime_dir}")
        else()
        endif()
        set(${runtime_var_name} "${${runtime_var_name}}" CACHE PATH "Output directory for executables and DLLs." FORCE)
    endforeach()

endmacro()

