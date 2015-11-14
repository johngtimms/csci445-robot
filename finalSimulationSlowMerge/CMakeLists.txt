cmake_minimum_required(VERSION 2.8)
project( particle )
SET(COMPILE_DEFINITIONS -Werror)
#OPENCV
find_package( OpenCV REQUIRED )

#set(HEADER_FILES Util.h Particle.h)

#add_library(mylib libsrc.cpp ${HEADER_FILES})
#add_library(mylib ${HEADER_FILES})
#add_executable(particle main.cpp ${HEADER_FILES})

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include )
link_directories( ${CMAKE_BINARY_DIR}/bin)

add_executable(particle main.cpp Util.h Particle.h)


target_link_libraries(particle ${OpenCV_LIBS})
