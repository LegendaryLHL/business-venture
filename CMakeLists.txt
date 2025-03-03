cmake_minimum_required(VERSION 3.10)

project(BusinessVenture)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(external/SDL)

execute_process(
    COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

FILE(GLOB_RECURSE SOURCE_FILES src/*.cpp)
FILE(GLOB_RECURSE EXTERNAL_FILES external/*.c external/*.cpp)

include_directories(external)

add_library(ExternalLib ${EXTERNAL_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw ExternalLib glm::glm SDL2::SDL2 SDL2::SDL2_mixer)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

add_custom_target(run
    COMMAND ./${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
)

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)

add_dependencies(${PROJECT_NAME} copy_assets)
