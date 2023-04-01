cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(core
    LANGUAGES CXX)

add_library(${PROJECT_NAME})
add_library(pork::core ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
    OUTPUT_NAME pork-core)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
    spdlog::spdlog
    fmt::fmt
    PUBLIC
    pork::common
    )
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<TARGET_PROPERTY:spdlog::spdlog,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>
    )
add_subdirectory(src)
