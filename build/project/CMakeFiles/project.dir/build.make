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
CMAKE_SOURCE_DIR = /home/bangme/Project_C

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bangme/Project_C/build

# Include any dependencies generated for this target.
include project/CMakeFiles/project.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include project/CMakeFiles/project.dir/compiler_depend.make

# Include the progress variables for this target.
include project/CMakeFiles/project.dir/progress.make

# Include the compile flags for this target's objects.
include project/CMakeFiles/project.dir/flags.make

project/CMakeFiles/project.dir/src/database.c.o: project/CMakeFiles/project.dir/flags.make
project/CMakeFiles/project.dir/src/database.c.o: ../project/src/database.c
project/CMakeFiles/project.dir/src/database.c.o: project/CMakeFiles/project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bangme/Project_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object project/CMakeFiles/project.dir/src/database.c.o"
	cd /home/bangme/Project_C/build/project && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT project/CMakeFiles/project.dir/src/database.c.o -MF CMakeFiles/project.dir/src/database.c.o.d -o CMakeFiles/project.dir/src/database.c.o -c /home/bangme/Project_C/project/src/database.c

project/CMakeFiles/project.dir/src/database.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/project.dir/src/database.c.i"
	cd /home/bangme/Project_C/build/project && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bangme/Project_C/project/src/database.c > CMakeFiles/project.dir/src/database.c.i

project/CMakeFiles/project.dir/src/database.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/project.dir/src/database.c.s"
	cd /home/bangme/Project_C/build/project && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bangme/Project_C/project/src/database.c -o CMakeFiles/project.dir/src/database.c.s

project/CMakeFiles/project.dir/src/main.c.o: project/CMakeFiles/project.dir/flags.make
project/CMakeFiles/project.dir/src/main.c.o: ../project/src/main.c
project/CMakeFiles/project.dir/src/main.c.o: project/CMakeFiles/project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bangme/Project_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object project/CMakeFiles/project.dir/src/main.c.o"
	cd /home/bangme/Project_C/build/project && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT project/CMakeFiles/project.dir/src/main.c.o -MF CMakeFiles/project.dir/src/main.c.o.d -o CMakeFiles/project.dir/src/main.c.o -c /home/bangme/Project_C/project/src/main.c

project/CMakeFiles/project.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/project.dir/src/main.c.i"
	cd /home/bangme/Project_C/build/project && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bangme/Project_C/project/src/main.c > CMakeFiles/project.dir/src/main.c.i

project/CMakeFiles/project.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/project.dir/src/main.c.s"
	cd /home/bangme/Project_C/build/project && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bangme/Project_C/project/src/main.c -o CMakeFiles/project.dir/src/main.c.s

# Object files for target project
project_OBJECTS = \
"CMakeFiles/project.dir/src/database.c.o" \
"CMakeFiles/project.dir/src/main.c.o"

# External object files for target project
project_EXTERNAL_OBJECTS =

project/project: project/CMakeFiles/project.dir/src/database.c.o
project/project: project/CMakeFiles/project.dir/src/main.c.o
project/project: project/CMakeFiles/project.dir/build.make
project/project: /usr/lib/x86_64-linux-gnu/libmysqlclient.so
project/project: project/CMakeFiles/project.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bangme/Project_C/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable project"
	cd /home/bangme/Project_C/build/project && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/project.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
project/CMakeFiles/project.dir/build: project/project
.PHONY : project/CMakeFiles/project.dir/build

project/CMakeFiles/project.dir/clean:
	cd /home/bangme/Project_C/build/project && $(CMAKE_COMMAND) -P CMakeFiles/project.dir/cmake_clean.cmake
.PHONY : project/CMakeFiles/project.dir/clean

project/CMakeFiles/project.dir/depend:
	cd /home/bangme/Project_C/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bangme/Project_C /home/bangme/Project_C/project /home/bangme/Project_C/build /home/bangme/Project_C/build/project /home/bangme/Project_C/build/project/CMakeFiles/project.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : project/CMakeFiles/project.dir/depend

