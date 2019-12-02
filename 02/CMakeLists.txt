cmake_minimum_required(VERSION 3.0)

project(02)

# Create a symbolic link to resources required by the executable
ADD_CUSTOM_TARGET(resources_02 ALL COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/resources ${PROJECT_BINARY_DIR}/resources)

add_executable(02
    src/main.cpp)