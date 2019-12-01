cmake_minimum_required(VERSION 3.0)

project(Demo)

# Create a symbolic link to resources required by the executable
ADD_CUSTOM_TARGET(resources ALL COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/resources ${PROJECT_BINARY_DIR}/resources)

add_executable(01
    src/main.cpp)