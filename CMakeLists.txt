cmake_minimum_required(VERSION 2.8.9)
project(bagofwords)

#INCLUDE
include_directories(.)
#include_directories(include)

#SRC
file(GLOB SOURCES "./*.cpp")

#flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fopenmp")

# Static Library
set ( PROJECT_LINK_LIBS libFL.a )
link_directories( ./libFL/lib )

include_directories(./libFL/include)


add_executable(bagofwords ${SOURCES})
target_link_libraries(bagofwords ${PROJECT_LINK_LIBS} )
