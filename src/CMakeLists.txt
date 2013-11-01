cmake_minimum_required(VERSION 2.6.0)
project(schumacher)

IF(UNIX)
	SET(CMAKE_CXX_FLAGS "-std=c++11 -Werror -Wall -Werror=sign-compare -Wno-error=unused-variable -Wno-error=unused-but-set-variable")
ENDIF(UNIX)

FILE(GLOB HEADERS  ./*.h*)

FILE(GLOB SRCS  ./*.cpp)

option (USE_OPENGL
  "Use openGL to render scene while calculating" ON)

list(APPEND schumacher_INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}/externals
	${CMAKE_SOURCE_DIR}/externals/glm
	${CMAKE_SOURCE_DIR}/externals/assimp
 	${CMAKE_SOURCE_DIR}/externals/FreeImage  
)

if (USE_OPENGL)
  list(APPEND schumacher_INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}/externals/GL
	${CMAKE_SOURCE_DIR}/externals/GL/GL)
  set(EXTRA_LIBS ${EXTRA_LIBS} GL glfw glut)
  set(EXTRA_LIB_FILES ${EXTRA_LIB_FILES} ${CMAKE_SOURCE_DIR}/externals/GL/lib)
  add_definitions(-DUSE_OPENGL)
endif (USE_OPENGL)

# Random generator stuff
list(APPEND schumacher_INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}/sfmt
)

FILE(GLOB_RECURSE SFMT_H ${CMAKE_SOURCE_DIR}/sfmt/*.h*)
FILE(GLOB_RECURSE SFMT_CPP ${CMAKE_SOURCE_DIR}/sfmt/*.c)

list(APPEND HEADERS ${SFMT_H})

list(APPEND SRCS ${SFMT_CPP})

add_definitions(-DSFMT_MEXP=19937)

####

include_directories(	
	${schumacher_INCLUDE_DIRS}
	)

link_directories(
  ${EXTRA_LIB_FILES}
	${CMAKE_SOURCE_DIR}/externals/libs
	${EXTRA_LIB_FILES}
	) 

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/export)

if(CMAKE_CURRENT_BINARY_DIR)
	add_definitions(-DCMAKE_BINARY_DIR=\"${CMAKE_CURRENT_BINARY_DIR}\")
else()
	add_definitions(-DCMAKE_BINARY_DIR=\"\")
endif()

add_executable(schumacher ${HEADERS} ${SRCS} )

if(WIN32)
	target_link_libraries(schumacher GLFW.lib FreeImage.lib freeglut.lib)
endif(WIN32)

if(UNIX)
	target_link_libraries(schumacher freeimage ${EXTRA_LIBS})
endif(UNIX)


