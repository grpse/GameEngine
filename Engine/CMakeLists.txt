
project(Engine)

cmake_minimum_required(VERSION 3.0)

# SET C++14 STANDARD
set (CMAKE_CXX_STANDARD 14)

# ALL SOURCES FOR COMPILATION AND LINKING
file(GLOB Engine_SOURCES 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
	${PROJECT_SOURCE_DIR}/include/*.h
)

# ALL DEPENDENCIES
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/external)
include_directories(${SDL2_SOURCE_DIR}/include)
include_directories(${SDL2_BINARY_DIR}/include)
include_directories(${glew_SOURCE_DIR}/include)
include_directories(${glm_SOURCE_DIR})
include_directories(${SOIL_SOURCE_DIR}/src)

# LINK GLEW STATICALLY
add_definitions(-DGLEW_STATIC)

# LINK LIBRARIES
if (WIN32)
    # SETUP EXPORT DLL FOR WINDOWS
    #target_compile_definitions(Engine PRIVATE ENGINE_EXPORTING)
    add_definitions(-DENGINE_EXPORTING)
    set(SDL2_LIBRARIES 
        SDL2-static
        SDL2main
        gdi32
		Kernel32
        user32
        opengl32
    )
    add_definitions("-std=c++14")
    add_definitions("-ggdb")
endif (WIN32)

if (UNIX)
    set(SDL2_LIBRARIES 
        SDL2-static
        SDL2main
        GL
        GLU
    )
    add_definitions("-std=c++14")
    add_definitions("-ggdb")
endif (UNIX)

message("Project Name: " ${PROJECT_NAME})
message("CPP FILES: " ${Engine_SOURCES})

add_library(Engine SHARED ${Engine_SOURCES})
target_link_libraries(Engine ${SDL2_LIBRARIES} glew SOIL)
