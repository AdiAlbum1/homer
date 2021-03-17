cmake_minimum_required(VERSION 3.0.0)

project(homer VERSION 1.0.0)

add_library(homer send_home.cpp)

#find cppzmq wrapper, installed by make of cppzmq
find_package(cppzmq)
#find nlohmann_json, installed by make of nlohmann
find_package(nlohmann_json)

target_link_libraries(homer cppzmq)
target_link_libraries(homer nlohmann_json::nlohmann_json)


add_executable(simp example/send_home_test.cpp)
include_directories(simp ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(simp homer cppzmq nlohmann_json::nlohmann_json)

execute_process (
    COMMAND bash -c "ln -snr ../configuration.json ."
)
