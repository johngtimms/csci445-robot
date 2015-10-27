cmake_minimum_required(VERSION 2.8)
project( Flex )
SET(COMPILE_DEFINITIONS -Werror)
#OPENCV
find_package( OpenCV REQUIRED )

add_executable(Flex flex.cpp)
target_link_libraries(Flex /opt/vc/lib/libmmal_core.so /opt/vc/lib/libmmal_util.so /opt/vc/lib/libmmal_vc_client.so /opt/vc/lib/libvcos.so /opt/vc/lib/libbcm_host.so wiringPi ${OpenCV_LIBS})

