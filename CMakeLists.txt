cmake_minimum_required(VERSION "3.19.2")

option(PRODUCTION_BUILD "Build in production mode" OFF)

project(OpenGLTest)
set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB_RECURSE STB_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/external/std_image/stb_image.cpp")
file(GLOB_RECURSE IMGUI CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/*.cpp")

list(APPEND MY_SOURCES "${STB_SOURCES}")
list(APPEND MY_SOURCES "${IMGUI}")

add_executable(OpenGLTest "${MY_SOURCES}")

if(PRODUCTION_BUILD)
target_compile_definitions(OpenGLTest PRIVATE ASSETS_PATH="assets/")
target_compile_definitions(OpenGLTest PRIVATE PROJECT_PATH="./")
else()
target_compile_definitions(OpenGLTest PRIVATE PROJECT_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(OpenGLTest PRIVATE ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets")
endif()

target_include_directories(OpenGLTest PRIVATE "src")
target_include_directories(OpenGLTest PRIVATE "external/std_image")
target_include_directories(OpenGLTest PRIVATE "external/glm")
target_include_directories(OpenGLTest PRIVATE "external/imgui")

target_link_libraries(OpenGLTest PRIVATE OpenGL::GL GLEW::GLEW glfw)

if (UNIX)
    target_link_libraries(OpenGLTest PRIVATE m pthread dl rt X11)
endif()

if(PRODUCTION_BUILD)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()
