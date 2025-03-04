cmake_minimum_required(VERSION 3.10)

project(BusinessVenture)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2_mixer REQUIRED IMPORTED_TARGET SDL2_mixer)

FILE(GLOB_RECURSE SOURCE_FILES src/*.cpp)
FILE(GLOB_RECURSE EXTERNAL_FILES external/*.c external/*.cpp)

include_directories(external)

add_library(ExternalLib ${EXTERNAL_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw ExternalLib glm::glm SDL2::SDL2 PkgConfig::SDL2_mixer)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)

add_dependencies(${PROJECT_NAME} copy_assets)

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
)
