set(CMAKE_SYSTEM_NAME Windows)
string(TOLOWER "${CMAKE_SYSTEM_NAME}" PORK_SYSTEM)
set(PORK_PLATFORM "x86_64")
set(PORK_BITNESS 64)

set(CMAKE_C_COMPILER   x86_64-w64-mingw32-gcc-win32)
set(CMAKE_CXX_COMPILER x86_64-w64-wingw32-g++-win32)

set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

