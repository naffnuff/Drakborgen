cmake_minimum_required(VERSION 3.20)
project(Drakborgen VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML - note the capitalized component names
find_package(SFML COMPONENTS system window graphics audio network CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML - these targets are correct
target_link_libraries(${PROJECT_NAME} 
    sfml-system 
    sfml-graphics 
    sfml-window
    sfml-audio
    sfml-network
)

# Set startup project properties
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})