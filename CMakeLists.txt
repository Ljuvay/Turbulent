cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fix GLFW platform issues
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)

# Dependencies
add_subdirectory(external/glfw)

add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

find_package(OpenGL REQUIRED)

# Adding OpenMP
find_package(OpenMP REQUIRED)

# App
file(GLOB_RECURSE APP_SOURCES "src/*.cpp")
add_executable(app ${APP_SOURCES})

# Gather include
file(GLOB_RECURSE ALL_HEADERS "src/*.h")
set(INCLUDE_DIRS "")
foreach(HEADER ${ALL_HEADERS})
    get_filename_component(DIR ${HEADER} DIRECTORY)
    list(APPEND INCLUDE_DIRS ${DIR})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

target_include_directories(app PUBLIC
    ${INCLUDE_DIRS}
    external/glad/include
    external/glfw/include
    external/glm
)

target_link_libraries(app PRIVATE
    glfw
    glad
    OpenGL::GL
    OpenMP::OpenMP_CXX
)

# Copy resources (optional, but fine)
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:app>/resources
)

