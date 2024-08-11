cmake_minimum_required(VERSION 3.15)
project(Planet_2 VERSION 0.1.0)

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.12.2)
FetchContent_MakeAvailable(EnTT)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE TARGET_SRC "./src/planet/*.cpp" )

add_executable(Planet ${TARGET_SRC} "./src/main.cpp")

target_link_libraries(Planet EnTT sfml-graphics sfml-system sfml-window )

target_include_directories(Planet PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/src/planet"
                            )
