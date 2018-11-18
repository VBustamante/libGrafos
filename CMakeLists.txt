cmake_minimum_required(VERSION 3.6)
project(LibGrafos)

set(CMAKE_CXX_STANDARD 11)

# Windows mingw linker options
#set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -mwindows")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
# Show all makefile commands
#set(CMAKE_VERBOSE_MAKEFILE ON)


# Windows Threading lib
# include_directories(libs/mingw-std-threads-master)

# Copy Resources
file(COPY graphs DESTINATION ./)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DSI_DEBUG)
endif()


add_executable(LibGrafos ${SOURCE_FILES} main.cpp Graph.cpp Graph.h GetTimeMs64.cpp tinyfiledialogs.c EuclidianGraph.cpp EuclidianGraph.h)

target_link_libraries(LibGrafos)
