cmake_minimum_required(VERSION 3.7)

include(ExternalProject)

project(generals_game)

if (UNIX)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)
endif()

set(GLFW_INSTALL_DIR "${CMAKE_BINARY_DIR}/glfw")
set(GLFW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include")
set(GLFW_LIBRARIES "${GLFW_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(SINGLE_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/single_headers")

ExternalProject_Add(glfw
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glfw"

    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""

    CMAKE_ARGS
        "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/glfw"
        "-DGLFW_BUILD_EXAMPLES=OFF"
        "-DGLFW_BUILD_TESTS=OFF"
        "-DGLFW_BUILD_DOCS=OFF"
        "-DBUILD_SHARED_LIBS=OFF"

    BUILD_BYPRODUCTS ${GLFW_LIBRARIES}
)

if (UNIX)
    set(GLFW_LIBRARIES ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${X11_LIBRARIES} ${X11_Xrandr_LIB} ${X11_Xcursor_LIB} ${X11_Xinerama_LIB} ${X11_Xxf86vm_LIB} ${CMAKE_THREAD_LIBS_INIT})
endif()

set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include")
add_library(glad "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c")
target_include_directories(glad PRIVATE "${GLAD_INCLUDE_DIR}")

set(GLAD_LIBRARIES)
if (UNIX)
    set(GLAD_LIBRARIES "glad" ${CMAKE_DL_LIBS})
else()
    set(GLAD_LIBRARIES glad)
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set (GCC ON)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL CLANG)
    set (CLANG ON)
endif()

if (GCC OR CLANG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall")
endif()



add_subdirectory(src)

