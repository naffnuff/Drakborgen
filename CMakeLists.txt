cmake_minimum_required(VERSION 3.20)
project(Drakborgen VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.x # or a specific version like 2.6.1
)

set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(SFML_INSTALL_PKGCONFIG_FILES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(sfml)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-audio
    sfml-network
    sfml-system
)

# Symlink compile_commands.json to the project root
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(SOURCE_CDB "${CMAKE_BINARY_DIR}/compile_commands.json")
  set(TARGET_CDB "${CMAKE_SOURCE_DIR}/compile_commands.json")

  message(STATUS "Linking compile_commands.json: ${TARGET_CDB} -> ${SOURCE_CDB}")

  execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${SOURCE_CDB}" "${TARGET_CDB}"
    )
endif()
