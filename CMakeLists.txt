cmake_minimum_required(VERSION 3.6)
project(LibGrafos)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

# Show all makefile commands
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/glfw-3.2.1)
include_directories(libs/glfw-3.2.1/include)

add_subdirectory(libs/freetype-2.7)
include_directories(libs/freetype-2.7/include)

add_subdirectory(libs/gl3w-master)
include_directories(libs/gl3w-master/include)

add_subdirectory(libs/glm-0.9.9-a2)
include_directories(libs/glm-0.9.9-a2/glm)

# Copy Resources
file(COPY graphs DESTINATION ./)
file(COPY shaders DESTINATION ./)
file(COPY fonts DESTINATION ./)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DSI_DEBUG)
endif()

set(SOURCE_FILES main.cpp Graph.cpp GetTimeMs64.cpp EuclidianGraph.cpp Screen.cpp sleep.cpp)
add_executable(LibGrafos ${SOURCE_FILES})

target_link_libraries(LibGrafos glfw ${GLFW_LIBRARIES} gl3w ${OPENGL_gl_LIBRARY} freetype glm)
