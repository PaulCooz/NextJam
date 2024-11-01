cmake_minimum_required(VERSION 3.0)
project(NextJam)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# don't build the supplied examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# don't build the supplied example games
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/build)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "5.0"
    GIT_PROGRESS TRUE
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/raylib
)
FetchContent_MakeAvailable(raylib)

# Adding our source files
# Define PROJECT_SOURCES as a list of all source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/code/*.cpp")

# Define PROJECT_INCLUDE to be the path to the include directory of the project
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/code/")

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")

    # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# add assets to build
set(RES_DIR "res")
file(COPY "${CMAKE_SOURCE_DIR}/${RES_DIR}" DESTINATION ${CMAKE_BINARY_DIR})

if(EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--preload-file ${RES_DIR}")
endif()
