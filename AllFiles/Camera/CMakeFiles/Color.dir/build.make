# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/Camera

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/Camera

# Include any dependencies generated for this target.
include CMakeFiles/Color.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Color.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Color.dir/flags.make

CMakeFiles/Color.dir/camcv.cpp.o: CMakeFiles/Color.dir/flags.make
CMakeFiles/Color.dir/camcv.cpp.o: camcv.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/Camera/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Color.dir/camcv.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Color.dir/camcv.cpp.o -c /home/pi/Camera/camcv.cpp

CMakeFiles/Color.dir/camcv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Color.dir/camcv.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/Camera/camcv.cpp > CMakeFiles/Color.dir/camcv.cpp.i

CMakeFiles/Color.dir/camcv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Color.dir/camcv.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/Camera/camcv.cpp -o CMakeFiles/Color.dir/camcv.cpp.s

CMakeFiles/Color.dir/camcv.cpp.o.requires:
.PHONY : CMakeFiles/Color.dir/camcv.cpp.o.requires

CMakeFiles/Color.dir/camcv.cpp.o.provides: CMakeFiles/Color.dir/camcv.cpp.o.requires
	$(MAKE) -f CMakeFiles/Color.dir/build.make CMakeFiles/Color.dir/camcv.cpp.o.provides.build
.PHONY : CMakeFiles/Color.dir/camcv.cpp.o.provides

CMakeFiles/Color.dir/camcv.cpp.o.provides.build: CMakeFiles/Color.dir/camcv.cpp.o

# Object files for target Color
Color_OBJECTS = \
"CMakeFiles/Color.dir/camcv.cpp.o"

# External object files for target Color
Color_EXTERNAL_OBJECTS =

Color: CMakeFiles/Color.dir/camcv.cpp.o
Color: CMakeFiles/Color.dir/build.make
Color: /opt/vc/lib/libmmal_core.so
Color: /opt/vc/lib/libmmal_util.so
Color: /opt/vc/lib/libmmal_vc_client.so
Color: /opt/vc/lib/libvcos.so
Color: /opt/vc/lib/libbcm_host.so
Color: /opt/vc/lib/libmmal_core.so
Color: /opt/vc/lib/libmmal_util.so
Color: /opt/vc/lib/libmmal_vc_client.so
Color: /opt/vc/lib/libvcos.so
Color: /opt/vc/lib/libbcm_host.so
Color: CMakeFiles/Color.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable Color"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Color.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Color.dir/build: Color
.PHONY : CMakeFiles/Color.dir/build

CMakeFiles/Color.dir/requires: CMakeFiles/Color.dir/camcv.cpp.o.requires
.PHONY : CMakeFiles/Color.dir/requires

CMakeFiles/Color.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Color.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Color.dir/clean

CMakeFiles/Color.dir/depend:
	cd /home/pi/Camera && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/Camera /home/pi/Camera /home/pi/Camera /home/pi/Camera /home/pi/Camera/CMakeFiles/Color.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Color.dir/depend

