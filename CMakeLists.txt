cmake_minimum_required(VERSION 2.8.9)
project(bagofwords)

#Select as source files all files in ./ with *.cpp or *.h
file(GLOB SOURCES "./*.cpp" "./*.h")

#Include directories
include_directories(.)
include_directories(./libFL/include/)
include_directories(./libFL/externals/libpng/include/)
include_directories(./libFL/externals/libjpeg/include/)
include_directories(./libFL/externals/libtiff/include/)
include_directories(./libFL/externals/libsvm/include/)
include_directories(/usr/local/include/opencv/)

#Configure important flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fopenmp -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_imgcodecs")

#Set static library path
link_directories(./libFL/lib)
link_directories(/usr/local/lib)
#Create executable
add_executable(bagofwords ${SOURCES})
#Link libraries
target_link_libraries(bagofwords libFL.a)
