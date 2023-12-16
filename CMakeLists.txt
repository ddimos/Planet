cmake_minimum_required(VERSION 3.0.0)
project(new_project VERSION 0.1.0)

# Add SFML 
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# Add EnTT 3.3.0 as a submodule
# URL: https://github.com/skypjack/entt.git
set(USE_LIBCPP OFF)
set(ENTT_BUILD_TESTING OFF)
add_subdirectory(external/entt)

file(GLOB_RECURSE TARGET_SRC "./src/planet/*.cpp" )

add_executable(Planet ${TARGET_SRC} "./src/main.cpp")

target_link_libraries(Planet EnTT sfml-graphics sfml-audio sfml-network)

target_include_directories(Planet PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/external/entt/src"
                            "${PROJECT_SOURCE_DIR}/src/planet"
                            )