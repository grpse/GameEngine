project(GameEngine_ParticleSystemTest)

cmake_minimum_required(VERSION 3.0)

file(GLOB SOURCES src/*.cpp)

if(UNIX)
    add_definitions("-std=c++14")
    add_definitions("-ggdb")
endif()

include_directories(${Engine_SOURCE_DIR}/include)
include_directories(${Engine_SOURCE_DIR}/external)

message("Engine Source Dir: " ${Engine_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Engine)