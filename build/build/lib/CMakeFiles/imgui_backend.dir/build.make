# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/tomasz/projects/sniffer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tomasz/projects/sniffer/build/build

# Include any dependencies generated for this target.
include lib/CMakeFiles/imgui_backend.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/CMakeFiles/imgui_backend.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/CMakeFiles/imgui_backend.dir/progress.make

# Include the compile flags for this target's objects.
include lib/CMakeFiles/imgui_backend.dir/flags.make

lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.o: lib/CMakeFiles/imgui_backend.dir/flags.make
lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.o: ../../lib/backends/imgui_impl_glfw.cpp
lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.o: lib/CMakeFiles/imgui_backend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tomasz/projects/sniffer/build/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.o"
	cd /home/tomasz/projects/sniffer/build/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.o -MF CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.o.d -o CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.o -c /home/tomasz/projects/sniffer/lib/backends/imgui_impl_glfw.cpp

lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.i"
	cd /home/tomasz/projects/sniffer/build/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tomasz/projects/sniffer/lib/backends/imgui_impl_glfw.cpp > CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.i

lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.s"
	cd /home/tomasz/projects/sniffer/build/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tomasz/projects/sniffer/lib/backends/imgui_impl_glfw.cpp -o CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.s

lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.o: lib/CMakeFiles/imgui_backend.dir/flags.make
lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.o: ../../lib/backends/imgui_impl_opengl3.cpp
lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.o: lib/CMakeFiles/imgui_backend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tomasz/projects/sniffer/build/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.o"
	cd /home/tomasz/projects/sniffer/build/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.o -MF CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.o.d -o CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.o -c /home/tomasz/projects/sniffer/lib/backends/imgui_impl_opengl3.cpp

lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.i"
	cd /home/tomasz/projects/sniffer/build/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tomasz/projects/sniffer/lib/backends/imgui_impl_opengl3.cpp > CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.i

lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.s"
	cd /home/tomasz/projects/sniffer/build/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tomasz/projects/sniffer/lib/backends/imgui_impl_opengl3.cpp -o CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.s

# Object files for target imgui_backend
imgui_backend_OBJECTS = \
"CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.o" \
"CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.o"

# External object files for target imgui_backend
imgui_backend_EXTERNAL_OBJECTS =

lib/libimgui_backend.a: lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_glfw.cpp.o
lib/libimgui_backend.a: lib/CMakeFiles/imgui_backend.dir/backends/imgui_impl_opengl3.cpp.o
lib/libimgui_backend.a: lib/CMakeFiles/imgui_backend.dir/build.make
lib/libimgui_backend.a: lib/CMakeFiles/imgui_backend.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tomasz/projects/sniffer/build/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libimgui_backend.a"
	cd /home/tomasz/projects/sniffer/build/build/lib && $(CMAKE_COMMAND) -P CMakeFiles/imgui_backend.dir/cmake_clean_target.cmake
	cd /home/tomasz/projects/sniffer/build/build/lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/imgui_backend.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/CMakeFiles/imgui_backend.dir/build: lib/libimgui_backend.a
.PHONY : lib/CMakeFiles/imgui_backend.dir/build

lib/CMakeFiles/imgui_backend.dir/clean:
	cd /home/tomasz/projects/sniffer/build/build/lib && $(CMAKE_COMMAND) -P CMakeFiles/imgui_backend.dir/cmake_clean.cmake
.PHONY : lib/CMakeFiles/imgui_backend.dir/clean

lib/CMakeFiles/imgui_backend.dir/depend:
	cd /home/tomasz/projects/sniffer/build/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tomasz/projects/sniffer /home/tomasz/projects/sniffer/lib /home/tomasz/projects/sniffer/build/build /home/tomasz/projects/sniffer/build/build/lib /home/tomasz/projects/sniffer/build/build/lib/CMakeFiles/imgui_backend.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/CMakeFiles/imgui_backend.dir/depend

