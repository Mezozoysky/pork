cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(mc
    LANGUAGES CXX)

add_executable(${PROJECT_NAME})
add_executable(pork::demo ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
    OUTPUT_NAME pork-mc
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    pork::render
)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/src"
)
add_subdirectory(src)
