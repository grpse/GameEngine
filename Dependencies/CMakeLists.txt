cmake_minimum_required(VERSION 3.0)

project(All_Deps)

# Dependencies static
# SDL2
SET(SDL_SHARED OFF CACHE BOOL "Disabling SDL2 shared" FORCE)
SET(SDL_STATIC ON CACHE BOOL "Enabling SDL2 static" FORCE)
if (UNIX)
    add_definitions("-Wl,Bstatic -static -ldbus-1 -ldl -lm  -Wl,--no-undefined -pthread -lSDL2 -fPIC")
    add_definitions(-DX11_LIB=false)
endif(UNIX)

# GLEW
SET(GLEW_SHARED OFF CACHE BOOL "Disabling GLEW shared" FORCE)
SET(GLEW_STATIC ON CACHE BOOL "Enabling GLEW static" FORCE)


SET(SOIL_SHARED OFF CACHE BOOL "Disabling SOIL shared" FORCE)
SET(SOIL_STATIC ON CACHE BOOL "Enabling SOIL static" FORCE)

add_subdirectory(SDL2)
add_subdirectory(glew)
add_subdirectory(SOIL)