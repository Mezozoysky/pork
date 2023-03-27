cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(pork
    LANGUAGES CXX
    VERSION 0.1.0
)

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

set(PORK_BUILD_SHARED_LIBS_DEFAULT ${BUILD_SHARED_LIBS})
set(PORK_BUILD_UT_DEFAULT TRUE)
set(PORK_BUILD_HEADLESS_DEFAULT FALSE)

option(PORK_BUILD_SHARED_LIBS "Build Pork libraries as shared" ${PORK_BUILD_SHARED_LIBS_DEFAULT})
option(PORK_BUILD_UT "Build Pork unit tests" ${PORK_BUILD_UT_DEFAULT})
option(PORK_BIULD_HEADLESS "Build Pork in headless mode" ${PORK_BUILD_HEADLESS_DEFAULT})

set(PORK_GL_VERSION "Auto" CACHE STRING "Use given GL version for Pork build")
set_property(CACHE PORK_GL_VERSION PROPERTY STRINGS "Auto" "None" "Vulkan" "OpenGL3" "OpenGL2")

option(PORK_SDL_BUNDLED "Use bundled SDL2 library for build" TRUE)
option(PORK_FMT_BUNDLED "Use bundled fmt library for build" TRUE)
option(PORK_SPDLOG_BUNDLED "Use bundled spdlog library for build" TRUE)
option(PORK_PUGIXML_BUNDLED "Use bundled pugixml library for build" TRUE)

add_library(common INTERFACE)
add_library(pork::common ALIAS common)

target_compile_options(common
    INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/Wall>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra>
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_compile_definitions(common INTERFACE PORK_PLATFORM_LINUX=1)
    set(PORK_PLATFORM_LINUX TRUE)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_compile_definitions(common INTERFACE PORK_PLATFORM_WINDOWS=1)
    set(PORK_PLATFORM_WINDOWS TRUE)
else()
    message(FATAL_ERROR "Unsupported platform: \"${CMAKE_SYSTEM_NAME}\"")
endif()

add_subdirectory(dependencies)
add_subdirectory(sources)
