# CMAKE generated file: DO NOT EDIT!
# Generated by "MSYS Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /C/msys64/mingw64/bin/cmake.exe

# The command to remove a file.
RM = /C/msys64/mingw64/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /D/Projects/crypto-pals/s2c9

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /D/Projects/crypto-pals/s2c9/build

# Include any dependencies generated for this target.
include CMakeFiles/s2c9.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/s2c9.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/s2c9.dir/flags.make

CMakeFiles/s2c9.dir/s2c9.cpp.obj: CMakeFiles/s2c9.dir/flags.make
CMakeFiles/s2c9.dir/s2c9.cpp.obj: ../s2c9.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/D/Projects/crypto-pals/s2c9/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/s2c9.dir/s2c9.cpp.obj"
	/C/msys64/mingw64/bin/g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/s2c9.dir/s2c9.cpp.obj -c /D/Projects/crypto-pals/s2c9/s2c9.cpp

CMakeFiles/s2c9.dir/s2c9.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s2c9.dir/s2c9.cpp.i"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /D/Projects/crypto-pals/s2c9/s2c9.cpp > CMakeFiles/s2c9.dir/s2c9.cpp.i

CMakeFiles/s2c9.dir/s2c9.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s2c9.dir/s2c9.cpp.s"
	/C/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /D/Projects/crypto-pals/s2c9/s2c9.cpp -o CMakeFiles/s2c9.dir/s2c9.cpp.s

# Object files for target s2c9
s2c9_OBJECTS = \
"CMakeFiles/s2c9.dir/s2c9.cpp.obj"

# External object files for target s2c9
s2c9_EXTERNAL_OBJECTS =

s2c9.exe: CMakeFiles/s2c9.dir/s2c9.cpp.obj
s2c9.exe: CMakeFiles/s2c9.dir/build.make
s2c9.exe: C:/msys64/mingw64/lib/libprotobuf.dll.a
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/D/Projects/crypto-pals/s2c9/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable s2c9.exe"
	/C/msys64/mingw64/bin/cmake.exe -E rm -f CMakeFiles/s2c9.dir/objects.a
	/C/msys64/mingw64/bin/ar.exe cr CMakeFiles/s2c9.dir/objects.a $(s2c9_OBJECTS) $(s2c9_EXTERNAL_OBJECTS)
	/C/msys64/mingw64/bin/g++.exe    -Wl,--whole-archive CMakeFiles/s2c9.dir/objects.a -Wl,--no-whole-archive  -o s2c9.exe -Wl,--major-image-version,0,--minor-image-version,0  /C/msys64/mingw64/lib/libprotobuf.dll.a -lssl -lcrypto -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 

# Rule to build all files generated by this target.
CMakeFiles/s2c9.dir/build: s2c9.exe

.PHONY : CMakeFiles/s2c9.dir/build

CMakeFiles/s2c9.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/s2c9.dir/cmake_clean.cmake
.PHONY : CMakeFiles/s2c9.dir/clean

CMakeFiles/s2c9.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MSYS Makefiles" /D/Projects/crypto-pals/s2c9 /D/Projects/crypto-pals/s2c9 /D/Projects/crypto-pals/s2c9/build /D/Projects/crypto-pals/s2c9/build /D/Projects/crypto-pals/s2c9/build/CMakeFiles/s2c9.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/s2c9.dir/depend

