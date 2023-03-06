cmake_minimum_required(VERSION 3.0.0)
project(new_project VERSION 0.1.0)

file(GLOB_RECURSE TARGET_SRC "./src/*.cpp" )

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(Planet ${TARGET_SRC})

target_link_libraries(Planet sfml-graphics sfml-audio sfml-network)

target_include_directories(Planet PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/src"
                            )