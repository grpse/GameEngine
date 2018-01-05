if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
	add_definitions( "/MP" )
endif()


cmake_minimum_required(VERSION 3.0)
project(glew)

if (GLEW_STATIC)
	add_definitions(-DGLEW_STATIC)
endif()

### glew ###

set(GLEW_SOURCE
	${PROJECT_SOURCE_DIR}/src/glew.c
)

set(GLEW_HEADERS
	${PROJECT_SOURCE_DIR}/include/GL/glew.h
)

include_directories(${PROJECT_SOURCE_DIR}/include/)

if (GLEW_STATIC)
	add_library(glew STATIC ${GLEW_SOURCE} ${GLEW_INCLUDE})
endif()

if (GLEW_SHARED)
	add_library(glew SHARED ${GLEW_SOURCE} ${GLEW_INCLUDE})
endif()



target_link_libraries(glew
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)

