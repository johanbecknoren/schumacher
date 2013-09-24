cmake_minimum_required(VERSION 2.6.0)
project(schumacher)

IF(UNIX)
	MESSAGE(STATUS "La till c++11!")
	SET(CMAKE_CXX_FLAGS "-std=c++11 -Werror")
ENDIF(UNIX)

FILE(GLOB HEADERS  ./*.h*)

FILE(GLOB SRCS  ./*.cpp)

list(APPEND schumacher_INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}/externals
	${CMAKE_SOURCE_DIR}/externals/glm
	${CMAKE_SOURCE_DIR}/externals/GL
	${CMAKE_SOURCE_DIR}/externals/GL/GL
	${CMAKE_SOURCE_DIR}/externals/assimp
 	${CMAKE_SOURCE_DIR}/externals/FreeImage  
)

include_directories(	
	${schumacher_INCLUDE_DIRS}
	)

link_directories(
	${CMAKE_SOURCE_DIR}/externals/GL/lib
	${CMAKE_SOURCE_DIR}/externals/libs
	) 

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/export)

if(CMAKE_CURRENT_BINARY_DIR)
	add_definitions(-DCMAKE_BINARY_DIR=\"${CMAKE_CURRENT_BINARY_DIR}\")
else()
	add_definitions(-DCMAKE_BINARY_DIR=\"\")
endif()

add_executable(schumacher ${HEADERS} ${SRCS} )

if(WIN32)
	target_link_libraries(schumacher GLFW.lib FreeImage.lib freeglut.lib assimp.lib glew32.lib )
endif(WIN32)

if(UNIX)
	target_link_libraries(schumacher freeimage GL glfw glut)
endif(UNIX)


