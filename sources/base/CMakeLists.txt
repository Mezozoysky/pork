cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(base
    LANGUAGES CXX)

add_library(${PROJECT_NAME})
add_library(pork::base ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
    OUTPUT_NAME pork-base)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
    pork::context
    spdlog::spdlog
    fmt::fmt
    pugixml::static
    SDL3::SDL3-static
    )
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<TARGET_PROPERTY:pork::context,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:fmt::fmt,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:spdlog::spdlog,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:pugixml::static,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:SDL3::SDL3-static,INTERFACE_INCLUDE_DIRECTORIES>
    )
add_subdirectory(src)
