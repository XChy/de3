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
CMAKE_SOURCE_DIR = /home/xchy/Projects/de3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xchy/Projects/de3

# Include any dependencies generated for this target.
include src/tools/unoptgen/CMakeFiles/unoptgen.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/tools/unoptgen/CMakeFiles/unoptgen.dir/compiler_depend.make

# Include the progress variables for this target.
include src/tools/unoptgen/CMakeFiles/unoptgen.dir/progress.make

# Include the compile flags for this target's objects.
include src/tools/unoptgen/CMakeFiles/unoptgen.dir/flags.make

src/tools/unoptgen/CMakeFiles/unoptgen.dir/main.cpp.o: src/tools/unoptgen/CMakeFiles/unoptgen.dir/flags.make
src/tools/unoptgen/CMakeFiles/unoptgen.dir/main.cpp.o: src/tools/unoptgen/main.cpp
src/tools/unoptgen/CMakeFiles/unoptgen.dir/main.cpp.o: src/tools/unoptgen/CMakeFiles/unoptgen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xchy/Projects/de3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/tools/unoptgen/CMakeFiles/unoptgen.dir/main.cpp.o"
	cd /home/xchy/Projects/de3/src/tools/unoptgen && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/tools/unoptgen/CMakeFiles/unoptgen.dir/main.cpp.o -MF CMakeFiles/unoptgen.dir/main.cpp.o.d -o CMakeFiles/unoptgen.dir/main.cpp.o -c /home/xchy/Projects/de3/src/tools/unoptgen/main.cpp

src/tools/unoptgen/CMakeFiles/unoptgen.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/unoptgen.dir/main.cpp.i"
	cd /home/xchy/Projects/de3/src/tools/unoptgen && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xchy/Projects/de3/src/tools/unoptgen/main.cpp > CMakeFiles/unoptgen.dir/main.cpp.i

src/tools/unoptgen/CMakeFiles/unoptgen.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/unoptgen.dir/main.cpp.s"
	cd /home/xchy/Projects/de3/src/tools/unoptgen && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xchy/Projects/de3/src/tools/unoptgen/main.cpp -o CMakeFiles/unoptgen.dir/main.cpp.s

src/tools/unoptgen/CMakeFiles/unoptgen.dir/engine/Mutator.cpp.o: src/tools/unoptgen/CMakeFiles/unoptgen.dir/flags.make
src/tools/unoptgen/CMakeFiles/unoptgen.dir/engine/Mutator.cpp.o: src/tools/unoptgen/engine/Mutator.cpp
src/tools/unoptgen/CMakeFiles/unoptgen.dir/engine/Mutator.cpp.o: src/tools/unoptgen/CMakeFiles/unoptgen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xchy/Projects/de3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/tools/unoptgen/CMakeFiles/unoptgen.dir/engine/Mutator.cpp.o"
	cd /home/xchy/Projects/de3/src/tools/unoptgen && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/tools/unoptgen/CMakeFiles/unoptgen.dir/engine/Mutator.cpp.o -MF CMakeFiles/unoptgen.dir/engine/Mutator.cpp.o.d -o CMakeFiles/unoptgen.dir/engine/Mutator.cpp.o -c /home/xchy/Projects/de3/src/tools/unoptgen/engine/Mutator.cpp

src/tools/unoptgen/CMakeFiles/unoptgen.dir/engine/Mutator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/unoptgen.dir/engine/Mutator.cpp.i"
	cd /home/xchy/Projects/de3/src/tools/unoptgen && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xchy/Projects/de3/src/tools/unoptgen/engine/Mutator.cpp > CMakeFiles/unoptgen.dir/engine/Mutator.cpp.i

src/tools/unoptgen/CMakeFiles/unoptgen.dir/engine/Mutator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/unoptgen.dir/engine/Mutator.cpp.s"
	cd /home/xchy/Projects/de3/src/tools/unoptgen && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xchy/Projects/de3/src/tools/unoptgen/engine/Mutator.cpp -o CMakeFiles/unoptgen.dir/engine/Mutator.cpp.s

# Object files for target unoptgen
unoptgen_OBJECTS = \
"CMakeFiles/unoptgen.dir/main.cpp.o" \
"CMakeFiles/unoptgen.dir/engine/Mutator.cpp.o"

# External object files for target unoptgen
unoptgen_EXTERNAL_OBJECTS =

build/unoptgen: src/tools/unoptgen/CMakeFiles/unoptgen.dir/main.cpp.o
build/unoptgen: src/tools/unoptgen/CMakeFiles/unoptgen.dir/engine/Mutator.cpp.o
build/unoptgen: src/tools/unoptgen/CMakeFiles/unoptgen.dir/build.make
build/unoptgen: /usr/local/lib/libLLVMAggressiveInstCombine.a
build/unoptgen: /usr/local/lib/libLLVMAnalysis.a
build/unoptgen: /usr/local/lib/libLLVMBinaryFormat.a
build/unoptgen: /usr/local/lib/libLLVMBitReader.a
build/unoptgen: /usr/local/lib/libLLVMBitstreamReader.a
build/unoptgen: /usr/local/lib/libLLVMBitWriter.a
build/unoptgen: /usr/local/lib/libLLVMCFGuard.a
build/unoptgen: /usr/local/lib/libLLVMCore.a
build/unoptgen: /usr/local/lib/libLLVMCoroutines.a
build/unoptgen: /usr/local/lib/libLLVMFileCheck.a
build/unoptgen: /usr/local/lib/libLLVMInstCombine.a
build/unoptgen: /usr/local/lib/libLLVMInstrumentation.a
build/unoptgen: /usr/local/lib/libLLVMipo.a
build/unoptgen: /usr/local/lib/libLLVMIRPrinter.a
build/unoptgen: /usr/local/lib/libLLVMIRReader.a
build/unoptgen: /usr/local/lib/libLLVMLibDriver.a
build/unoptgen: /usr/local/lib/libLLVMLinker.a
build/unoptgen: /usr/local/lib/libLLVMObject.a
build/unoptgen: /usr/local/lib/libLLVMOption.a
build/unoptgen: /usr/local/lib/libLLVMPasses.a
build/unoptgen: /usr/local/lib/libLLVMScalarOpts.a
build/unoptgen: /usr/local/lib/libLLVMSupport.a
build/unoptgen: /usr/local/lib/libLLVMSymbolize.a
build/unoptgen: /usr/local/lib/libLLVMTarget.a
build/unoptgen: /usr/local/lib/libLLVMTargetParser.a
build/unoptgen: /usr/local/lib/libLLVMTextAPI.a
build/unoptgen: /usr/local/lib/libLLVMTransformUtils.a
build/unoptgen: /usr/local/lib/libLLVMVectorize.a
build/unoptgen: build/libUnoptGenCore.a
build/unoptgen: /usr/local/lib/libLLVMCFGuard.a
build/unoptgen: /usr/local/lib/libLLVMCoroutines.a
build/unoptgen: /usr/local/lib/libLLVMipo.a
build/unoptgen: /usr/local/lib/libLLVMInstrumentation.a
build/unoptgen: /usr/local/lib/libLLVMLinker.a
build/unoptgen: /usr/local/lib/libLLVMFrontendOpenMP.a
build/unoptgen: /usr/local/lib/libLLVMFrontendOffloading.a
build/unoptgen: /usr/local/lib/libLLVMIRPrinter.a
build/unoptgen: /usr/local/lib/libLLVMVectorize.a
build/unoptgen: /usr/local/lib/libLLVMCodeGen.a
build/unoptgen: /usr/local/lib/libLLVMBitWriter.a
build/unoptgen: /usr/local/lib/libLLVMScalarOpts.a
build/unoptgen: /usr/local/lib/libLLVMAggressiveInstCombine.a
build/unoptgen: /usr/local/lib/libLLVMInstCombine.a
build/unoptgen: /usr/local/lib/libLLVMTarget.a
build/unoptgen: /usr/local/lib/libLLVMCodeGenTypes.a
build/unoptgen: /usr/local/lib/libLLVMHipStdPar.a
build/unoptgen: /usr/local/lib/libLLVMObjCARCOpts.a
build/unoptgen: /usr/local/lib/libLLVMTransformUtils.a
build/unoptgen: /usr/local/lib/libLLVMAnalysis.a
build/unoptgen: /usr/local/lib/libLLVMProfileData.a
build/unoptgen: /usr/local/lib/libLLVMSymbolize.a
build/unoptgen: /usr/local/lib/libLLVMDebugInfoPDB.a
build/unoptgen: /usr/local/lib/libLLVMDebugInfoMSF.a
build/unoptgen: /usr/local/lib/libLLVMDebugInfoBTF.a
build/unoptgen: /usr/local/lib/libLLVMDebugInfoDWARF.a
build/unoptgen: /usr/local/lib/libLLVMObject.a
build/unoptgen: /usr/local/lib/libLLVMIRReader.a
build/unoptgen: /usr/local/lib/libLLVMBitReader.a
build/unoptgen: /usr/local/lib/libLLVMAsmParser.a
build/unoptgen: /usr/local/lib/libLLVMTextAPI.a
build/unoptgen: /usr/local/lib/libLLVMMCParser.a
build/unoptgen: /usr/local/lib/libLLVMMC.a
build/unoptgen: /usr/local/lib/libLLVMDebugInfoCodeView.a
build/unoptgen: /usr/local/lib/libLLVMCore.a
build/unoptgen: /usr/local/lib/libLLVMBinaryFormat.a
build/unoptgen: /usr/local/lib/libLLVMRemarks.a
build/unoptgen: /usr/local/lib/libLLVMBitstreamReader.a
build/unoptgen: /usr/local/lib/libLLVMTargetParser.a
build/unoptgen: /usr/local/lib/libLLVMSupport.a
build/unoptgen: /usr/local/lib/libLLVMDemangle.a
build/unoptgen: /usr/lib/x86_64-linux-gnu/libz.so
build/unoptgen: /usr/lib/x86_64-linux-gnu/libtinfo.so
build/unoptgen: src/tools/unoptgen/CMakeFiles/unoptgen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xchy/Projects/de3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../../build/unoptgen"
	cd /home/xchy/Projects/de3/src/tools/unoptgen && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/unoptgen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/tools/unoptgen/CMakeFiles/unoptgen.dir/build: build/unoptgen
.PHONY : src/tools/unoptgen/CMakeFiles/unoptgen.dir/build

src/tools/unoptgen/CMakeFiles/unoptgen.dir/clean:
	cd /home/xchy/Projects/de3/src/tools/unoptgen && $(CMAKE_COMMAND) -P CMakeFiles/unoptgen.dir/cmake_clean.cmake
.PHONY : src/tools/unoptgen/CMakeFiles/unoptgen.dir/clean

src/tools/unoptgen/CMakeFiles/unoptgen.dir/depend:
	cd /home/xchy/Projects/de3 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xchy/Projects/de3 /home/xchy/Projects/de3/src/tools/unoptgen /home/xchy/Projects/de3 /home/xchy/Projects/de3/src/tools/unoptgen /home/xchy/Projects/de3/src/tools/unoptgen/CMakeFiles/unoptgen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/tools/unoptgen/CMakeFiles/unoptgen.dir/depend
