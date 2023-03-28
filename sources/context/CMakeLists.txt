cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(context
  LANGUAGES CXX)

add_library(${PROJECT_NAME})
add_library(pork::context ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
    OUTPUT_NAME pork-context)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
    )
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
add_subdirectory(src)
