# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/augusto/projects/MITSP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/augusto/projects/MITSP/build-release

# Include any dependencies generated for this target.
include CMakeFiles/mitsp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/mitsp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/mitsp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mitsp.dir/flags.make

CMakeFiles/mitsp.dir/src/mainMITSP.cpp.o: CMakeFiles/mitsp.dir/flags.make
CMakeFiles/mitsp.dir/src/mainMITSP.cpp.o: /home/augusto/projects/MITSP/src/mainMITSP.cpp
CMakeFiles/mitsp.dir/src/mainMITSP.cpp.o: CMakeFiles/mitsp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/augusto/projects/MITSP/build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mitsp.dir/src/mainMITSP.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mitsp.dir/src/mainMITSP.cpp.o -MF CMakeFiles/mitsp.dir/src/mainMITSP.cpp.o.d -o CMakeFiles/mitsp.dir/src/mainMITSP.cpp.o -c /home/augusto/projects/MITSP/src/mainMITSP.cpp

CMakeFiles/mitsp.dir/src/mainMITSP.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/mitsp.dir/src/mainMITSP.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/augusto/projects/MITSP/src/mainMITSP.cpp > CMakeFiles/mitsp.dir/src/mainMITSP.cpp.i

CMakeFiles/mitsp.dir/src/mainMITSP.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/mitsp.dir/src/mainMITSP.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/augusto/projects/MITSP/src/mainMITSP.cpp -o CMakeFiles/mitsp.dir/src/mainMITSP.cpp.s

# Object files for target mitsp
mitsp_OBJECTS = \
"CMakeFiles/mitsp.dir/src/mainMITSP.cpp.o"

# External object files for target mitsp
mitsp_EXTERNAL_OBJECTS =

bin/mitsp: CMakeFiles/mitsp.dir/src/mainMITSP.cpp.o
bin/mitsp: CMakeFiles/mitsp.dir/build.make
bin/mitsp: CMakeFiles/mitsp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/augusto/projects/MITSP/build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/mitsp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mitsp.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -E copy_directory /home/augusto/projects/MITSP/instances /home/augusto/projects/MITSP/build-release/bin/instances

# Rule to build all files generated by this target.
CMakeFiles/mitsp.dir/build: bin/mitsp
.PHONY : CMakeFiles/mitsp.dir/build

CMakeFiles/mitsp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mitsp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mitsp.dir/clean

CMakeFiles/mitsp.dir/depend:
	cd /home/augusto/projects/MITSP/build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/augusto/projects/MITSP /home/augusto/projects/MITSP /home/augusto/projects/MITSP/build-release /home/augusto/projects/MITSP/build-release /home/augusto/projects/MITSP/build-release/CMakeFiles/mitsp.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/mitsp.dir/depend

