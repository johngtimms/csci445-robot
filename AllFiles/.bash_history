sudo raspi-config 
sudo reboot 
ls
cd /etc/network/
ls
sudo cp interfaces interfaces_origin
ls
cat interfaces
cat interfaces_origin 
sudo nano interfaces
sudo ifdown wlan0 
pwd
sudo nano interfaces
ls
cat interfaces
sudo ifup wlan0 
ls
sudo reboot 
sudo apt-get update
sudo apt-get install dhcp3-server
cd /etc/dhcp
ls
sudo cp dhcpd.conf dhcpd_origin.conf
sudo nano dhcpd.conf 
cd ../network/
ls
sudo ifdown wlan0 
sudo cp interfaces_modified interfaces
cd ../dhcp
ls
sudo cp dhcpd_modified.conf dhcpd.conf 
cat dhcpd.conf 
sudo ifup wlan0 
sudo reboot 
ls
exit
ls
exit
cd /etc/network/
ls
cat interfaces
sudo ifdown wlan0 
sudo cp interfaces_origin interfaces
cd ../dhcp
sudo cp dhcpd_origin.conf dhcpd.conf 
sudo ifup wlan0
sudo reboot 
ls
sudo poweroff 
sudo mkdir Sonar
cd Sonar/
sudo nano CMakeLists.txt
sudo nano sonar.cpp
sudo raspi-config 
ls
sudo raspi-config 
sudo reboot 
sudo raspi-config 
sudo reboot 
ls
sudo nano /etc/default/keyboard 
sudo reboot 
ls
cd Sonar/
ls
sudo nano sonar.cpp
ls
cd /etc/network/
ls
cat interfaces
sudo ifdown wlan0 
sudo cp interfaces_modified interfaces
cd ../dhcp
ls
sudo cp dhcpd_modified.conf dhcpd.conf 
ls
sudo ifup wlan0 
sudo reboot 
cd /etc/network/
sudo poweroff 
ls
cd Sonar/
sudo nano sonar.cpp
ls
sudo cmake .
sudo make
sudo ./Sonar 
ls
sudo apt-get -y install build-essential cmake cmake-curses-gui pkg
ls
cd /etc/network/
ls
cat interfaces
sudo ifdown wlan0 
sudo cp interfaces_origin interfaces
cd ../dhcp
sudo cp dhcpd_origin.conf dhcpd.conf 
sudo ifup wlan0 
sudo reboot 
ls
sudo apt-get -y install build-essential cmake cmake-curses-gui pkg
sudo apt-get -y install build-essential cmake-curses-gui pkg
sudo apt-get update
sudo apt-get install libopencv-dev 
sudo apt-get install python-opencv
sudo raspi-config 
ls
cd /etc/network/
sudo ifdown wlan0 
sudo cp in_modified interfaces
cd ../dhcp
sudo cp dhcpd_modified.conf dhcpd.conf 
sudo ifup wlan0 
sudo reboot 
ls
sudo mkdir displayImg
cd displayImg/
sudo nano CMakeLists.txt
sudo nano display_image.cpp
sudo cmake .
exit
cd /etc/network/
ls
sudo ifdown wlan0 
sudo cp interfaces_origin interfaces
cd ../dhcp
sudo cp dhcpd_origin.conf dhcpd.conf 
sudo ifup wlan0 
sudo reboot 
ls
cd displayImg/
ls
sudo nano display_image.cpp 
cd ~
ls
cd Desktop/
ls
sudo mv lion-wide.jpg ../displayImg/
cd ../displayImg/
ls
sudo make
sudo nano CMakeLists.txt 
sudo nano display_image.cpp 
cd ..
ls
cd /etc/
ls
cd /lib/
ls
cd /usr/lib/
ls
cd ..
ls
cd ~/displayImg/
ls
sudo nano CMakeLists.txt 
sudo poweroff 
cd displayImg/
ls
sudo rm display_image.cpp 
sudo nano display_image.cpp
sudo make 
ls
sudo ./displayimage lion-wide.jpg 
ls
cd ~/Desktop/
sudo mv Iron-Man.jpg ../displayImg/
ls
cd ../displayImg/
ls
sudo rm lion-wide.jpg 
sudo ./displayimage Iron-Man.jpg 
ls
sudo poweroff 
ls
cd /etc/network/
ls
cat interfaces
sudo ifdown wlan0 
sudo cp interfaces_modified interfaces
cd ../dhcp
sudo cp dhcpd_modified.conf dhcpd.conf 
sudo ifup wlan0 
sudo poweroff 
ls
sudo mkdir Compass
cd Compass/
sudo nano compass.cpp
g++ compass.cpp 
sudo g++ compass.cpp 
sudo ./a.out 
sudo nano compass.cpp 
sudo g++ compass.cpp 
sudo nano compass.cpp 
sudo rm compass.cpp 
exit
ls
cd Compass/
ls
sudo rm a.out 
ls
sudo nano compass.cpp
sudo g++ compass.cpp 
exit
ls
cd Compass/
ls
sudo nano compass.cpp 
sudo g++ compass.cpp 
sudo nano compass.cpp 
sudo rm compass.cpp 
sudo nano compass.cpp
ls
sudo nano compass.cpp
sudo g++ compass.cpp 
sudo ./a.out 
sudo nano /etc/modprobe.d/raspi-blacklist.conf 
sudo reboot 
ls
cd Compass/
ls
cat compass.cpp 
exit
ls
cd Compass/
ls
sudo com.cpp
sudo nano com.cpp
sudo g++ com.cpp 
ls
sudo rm a.out 
sudo g++ com.cpp 
sudo ./a.out 
sudo modprobe i2c-dev
sudo chmod o+rw /dev/i2c*
sudo ./a.out 
ls
sudo mkdir drawMap
cd drawMap/
sudo nano drawmap.cpp
sudo nano Util.h
sudo nano CMakeLists.txt
sudo nano map.txt
sudo cmake .
sudo mv drawmap.cpp drawMap.cpp 
ls
sudo cmake .
sudo make
sudo rm -rf CMakeCache.txt CMakeFiles/
ls
sudo rm -rf cmake_install.cmake Makefile 
sudo cmake .
sudo make
sudo poweroff 
ls
sudo mkdir particle
cd particle/
ls
sudo nano main.cpp
sudo nano Util.h
ls
sudo nano Particle.h
ls
sudo g++ main.cpp 
ls
sudo nano main.cpp 
sudo gcc main.cpp 
exit
ls
cd drawMap/
ls
sudo make
set lischars+=nbsp:.
sudo make
ls
tr -d "\240\302" <drawMap.cpp>drawMap.cpp
sudo tr -d "\240\302" <drawMap.cpp>drawMap.cpp
sudo tr -d "\240\302" <drawMap.cpp>drawMap2.cpp
sudo tr -d "\240\302" <drawMap.cpp> drawMap2.cpp
cd ..
sudo chmod 777 drawMap/
cd drawMap/
sudo tr -d "\240\302" <drawMap.cpp> drawMap2.cpp
ls
sudo mv drawMap2.cpp drawMap.cpp 
ls
sudo tr -d "\240\302" <Util.h> Util.h 
sudo tr -d "\240\302" <Util.h> Util2.h 
sudo mv Util2.h Util.h 
ls
cat drawMap.cpp 
cd ..
cd drawMap/
ls
sudo make
ls
sudo ./draw_map 
ls
sudo rm -rf CMakeCache.txt 
sudo rm -rf cmake_install.cmake Makefile CMakeFiles/
ls
sudo tr -d "\240\302" <CMakeLists.txt> CMakeLists2.txt 
sudo mv CMakeLists2.txt CMakeLists.txt 
ls
sudo tr -d "\240\302" <map.txt> map2.txt 
sudo mv map2.txt map.txt 
ls
sudo rm draw_map 
ls
sudo cmake .
sudo make
ls
sudo ./draw_map 
ls
cd /etc/network/
ls
cat interfaces
ls
cd ~
ls
cd drawMap/
ls
cd ..
ls
sudo mkdir test
cd test/
sudo nano test.cpp
sudo g++ test.cpp 
sudo ./a.out 
ls
sudo nano Util.h
ls
sudo nano test.cpp 
sudo g++ test.cpp 
sudo ./a.out 
sudo apt-get update
sudo poweroff 
ls
cd /etc/network/
ls
sudo cp interfaces_origin interfaces
sudo ifdown wlan0 
sudo cp interfaces_origin interfaces
cat interfaces
cd /etc/dhcp
ls
sudo cp dhcpd_origin.conf dhcpd.conf 
sudo ifup wlan0 
sudo reboot 
ls
sudo apt-get update 
sudo apt-get install tightvncserver 
vncserver :1
cd /home/pi/
cd .config/
sudo mkdir autostart
cd autostart/
sudo nano tightvnc.desktop
cd /etc/network/
ls
sudo ifdown wlan0 
cd ../dhcp
ls
sudo cp dhcpd_modified.conf dhcpd.conf 
cd ../network/
ls
sudo cp interfaces_modified interfaces
ls
cat interfaces
sudo ifup wlan0 
sudo reboot 
        exit
ls
cd particle/
ls
cat main.cpp 
sudo nano main.cpp 
sudo g++ main.cpp 
sudo nano main.cpp 
sudo g++ main.cpp 
sudo nano main.cpp 
cd ..
ls
cd test/
ls
cat test.cpp 
cd ..
ls
cd displayImg/
ls
cat display
cat display_image.cpp 
cd ..
ls
cd particle/
ls
cat main.cpp 
sudo nano main.cpp 
sudo g++ main.cpp 
ls
cd particle/
ls
sudo rm Util.h 
sudo nano Util.h
sudo nano Particle.h
sudo rm Particle.h 
ls
sudo rm main.cpp 
ls
sudo nano Particle.h
sudo nano main.cpp
ls
sudo g++ main.cpp 
sudo nano main.cpp 
sudo g++ main.cpp 
sudo nano main.cpp 
sudo g++ main.cpp 
ls
cd particle/
ls
sudo nano CMakeLists.txt 
ls
sudo nano CMakeLists.txt 
sudo cmake .
sudo make 
ls
sudo nano CMakeLists.txt 
sudo cmake .
sudo rm -rf CMakeFiles/ cmake_install.cmake Makefile 
ls
sudo rm CMakeCache.txt 
ls
sudo cmake .
sudo nano CMakeLists.txt 
sudo cmake .
sudo make 
ls
sudo nano main.cpp 
ls
sudo make
sudo nano main.cpp 
sudo make
exit
                                                                  ls
exit
ls
cd /etc/network/
ls
cat interfaces
sudo reboot 
ls
cd /etc/network/
ls
cd ..
ls
cd dhcp
ls
cat dhcpd.conf 
cd ../network/
ls
cat interfaces
cd ../dhcp
ls
sudo nano dhcpd.conf 
sudo ifdown wlan0 
sudo nano dhcpd.conf 
sudo ifup wlan0 
sudo reboot 
             sudo mkdir Flex
cd Flex/
sudo nano CMakeLists.txt
sudo nano flex.cpp
sudo cmake .
sudo make
sudo ./Flex
exit
ls
cd Sonar/
ls
sudo ./Sonar 
cd ~
ls
exit
ls
cd particle/
ls
cat CMakeLists.txt 
ls
cat main.cpp 
cd /etc/network/
ls
cat interfaces
exit
sudo apt-get install netatalk
sudo reboot 
ls
sudo rm -rf displayImg/
ls
sudo rm -rf drawMap/
ls
cd drawMap/
ls
sudo ./draw_map 
cd ..
ls
sudo rm -rf drawMap/
exit
ls
cd test/
ls
sudo ./test Iron-Man.jpg 
ls
cd ..
ls
cd particle/
ls
cd ..
                                                                                                                                ls
cd ServoMotor/
cd ..
ls
passwd 
ls
vncpasswd 
cd ServoMotor/
ls
sudo rm -rf CMakeCache.txt cmake_install.cmake Makefile CMakeFiles/ ServoMotor 
ls
sudo cmake .
ls
make 
sudo make 
sudo rm -rf CMakeCache.txt cmake_install.cmake Makefile CMakeFiles/ ServoMotor 
ls
sudo nano servomotor.cpp 
sudo cmake .
sudo make 
sudo ifdown wlan0 
cd /etc/network/
sudo nano interfaces
sudo ifup wlan0 
sudo poweroff 
ls
cd
vncserver
cs Sonar/
cd Sonar/
sudo ./Sonar
exit
          ls
vncserver
                                                                                                                                                                                                                                                                                                                 ls
cd Aidan/
ls
cd ..
ls
vncserver
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             ls
clear
ls
clear
ls
clear
ls
cd particleFilter/
ls
clear
ls
clear
ls
clear
ls
clear
ls
clear
ls
clear
ls
clear
ls
clear
ls
clear
ls
clear
ls
./particle
clear
ls
gedit openme.txt
clear
ls
openme.txt
clear
edit openme.txt
ls
nano particleFilter/
ls
clear
ls
cd particleFilter/
ls
clear
ls
cd particleFilter_temp/
ls
gmake
make
make main.cpp
clear
ls
make main.cpp
ls
gedit CMakeLists.txt
make CMakelists.txt
CMakeLists.txt
CMakelists.txt
clear
g++ main.cpp
clear
do make
make
ls
make main.cpp
make clean
make
clear
ls
make main.cpp
sudo make
ls
./particle
clear
.particle
./particle
vncserver
ls
cd Documents
ls
cd ..
cd particleFilter_temp/
ls
make
sudo make
make clean
cd ..
cd particleFilter
ls
make
clear
ls
cd ..
clear
cd particleFilter_temp
ls
make
clear
./particle
clear
ls
clean
make clean
clear
make
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            ls
cd particleFilter
ls
clear
ls
make
clear
touch *
sudo touch *
clear
ls
clear
make
./particle
ls
cd /etc
ls | grep "vnc"
ls
cd init.d
ls
cd ..
which vncserver
vncserver --help
cd /usr/opt
cd /usr/
ls
cd ..
ls
cd var/
ls
cd ..
cd opt
ls
cd vc/
ls
cd bin/
ls
cd ..
man vncserver
cd /etc/
ls
cd X11/
ls
ls -la
nano Xsession.options 
ls -la app-defaults/
ls
cd particleFilter
ls
./particle 
export DISPLAY=:1.0
./particle 
exit
ls
cd Aidan/Robot
ls
clear
ls
g++ manualOverride.cpp robot.h
g++ -std=c++0x manualOverride.cpp robot.h
clear
g++ -std=c++0x manualOverride.cpp robot.h
clear
g++ -std=c++0x manualOverride.cpp robot.h
clear
g++ -std=c++0x manualOverride.cpp robot.h
g++ robot.h
g++ -std=c++0x manualOverride.cpp robot.h
g++ -std=c++0x manualOverride.cpp robot.h robot.cpp
clear
g++ -std=c++0x manualOverride.cpp robot.h robot.cpp
clear
g++ -std=c++0x manualOverride.cpp robot.h robot.cpp
clear
g++ -std=c++0x manualOverride.cpp robot.h robot.cpp
clear
g++ -std=c++0x manualOverride.cpp robot.h robot.cpp
clear
g++ -std=c++0x manualOverride.cpp robot.h robot.cpp
ls
./a.out
