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
include src/tools/deoptgen/CMakeFiles/deoptgen.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/tools/deoptgen/CMakeFiles/deoptgen.dir/compiler_depend.make

# Include the progress variables for this target.
include src/tools/deoptgen/CMakeFiles/deoptgen.dir/progress.make

# Include the compile flags for this target's objects.
include src/tools/deoptgen/CMakeFiles/deoptgen.dir/flags.make

src/tools/deoptgen/CMakeFiles/deoptgen.dir/main.cpp.o: src/tools/deoptgen/CMakeFiles/deoptgen.dir/flags.make
src/tools/deoptgen/CMakeFiles/deoptgen.dir/main.cpp.o: src/tools/deoptgen/main.cpp
src/tools/deoptgen/CMakeFiles/deoptgen.dir/main.cpp.o: src/tools/deoptgen/CMakeFiles/deoptgen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xchy/Projects/de3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/tools/deoptgen/CMakeFiles/deoptgen.dir/main.cpp.o"
	cd /home/xchy/Projects/de3/src/tools/deoptgen && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/tools/deoptgen/CMakeFiles/deoptgen.dir/main.cpp.o -MF CMakeFiles/deoptgen.dir/main.cpp.o.d -o CMakeFiles/deoptgen.dir/main.cpp.o -c /home/xchy/Projects/de3/src/tools/deoptgen/main.cpp

src/tools/deoptgen/CMakeFiles/deoptgen.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/deoptgen.dir/main.cpp.i"
	cd /home/xchy/Projects/de3/src/tools/deoptgen && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xchy/Projects/de3/src/tools/deoptgen/main.cpp > CMakeFiles/deoptgen.dir/main.cpp.i

src/tools/deoptgen/CMakeFiles/deoptgen.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/deoptgen.dir/main.cpp.s"
	cd /home/xchy/Projects/de3/src/tools/deoptgen && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xchy/Projects/de3/src/tools/deoptgen/main.cpp -o CMakeFiles/deoptgen.dir/main.cpp.s

src/tools/deoptgen/CMakeFiles/deoptgen.dir/engine/Mutator.cpp.o: src/tools/deoptgen/CMakeFiles/deoptgen.dir/flags.make
src/tools/deoptgen/CMakeFiles/deoptgen.dir/engine/Mutator.cpp.o: src/tools/deoptgen/engine/Mutator.cpp
src/tools/deoptgen/CMakeFiles/deoptgen.dir/engine/Mutator.cpp.o: src/tools/deoptgen/CMakeFiles/deoptgen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xchy/Projects/de3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/tools/deoptgen/CMakeFiles/deoptgen.dir/engine/Mutator.cpp.o"
	cd /home/xchy/Projects/de3/src/tools/deoptgen && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/tools/deoptgen/CMakeFiles/deoptgen.dir/engine/Mutator.cpp.o -MF CMakeFiles/deoptgen.dir/engine/Mutator.cpp.o.d -o CMakeFiles/deoptgen.dir/engine/Mutator.cpp.o -c /home/xchy/Projects/de3/src/tools/deoptgen/engine/Mutator.cpp

src/tools/deoptgen/CMakeFiles/deoptgen.dir/engine/Mutator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/deoptgen.dir/engine/Mutator.cpp.i"
	cd /home/xchy/Projects/de3/src/tools/deoptgen && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xchy/Projects/de3/src/tools/deoptgen/engine/Mutator.cpp > CMakeFiles/deoptgen.dir/engine/Mutator.cpp.i

src/tools/deoptgen/CMakeFiles/deoptgen.dir/engine/Mutator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/deoptgen.dir/engine/Mutator.cpp.s"
	cd /home/xchy/Projects/de3/src/tools/deoptgen && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xchy/Projects/de3/src/tools/deoptgen/engine/Mutator.cpp -o CMakeFiles/deoptgen.dir/engine/Mutator.cpp.s

# Object files for target deoptgen
deoptgen_OBJECTS = \
"CMakeFiles/deoptgen.dir/main.cpp.o" \
"CMakeFiles/deoptgen.dir/engine/Mutator.cpp.o"

# External object files for target deoptgen
deoptgen_EXTERNAL_OBJECTS =

build/deoptgen: src/tools/deoptgen/CMakeFiles/deoptgen.dir/main.cpp.o
build/deoptgen: src/tools/deoptgen/CMakeFiles/deoptgen.dir/engine/Mutator.cpp.o
build/deoptgen: src/tools/deoptgen/CMakeFiles/deoptgen.dir/build.make
build/deoptgen: /usr/local/lib/libLLVMAggressiveInstCombine.a
build/deoptgen: /usr/local/lib/libLLVMAnalysis.a
build/deoptgen: /usr/local/lib/libLLVMBinaryFormat.a
build/deoptgen: /usr/local/lib/libLLVMBitReader.a
build/deoptgen: /usr/local/lib/libLLVMBitstreamReader.a
build/deoptgen: /usr/local/lib/libLLVMBitWriter.a
build/deoptgen: /usr/local/lib/libLLVMCFGuard.a
build/deoptgen: /usr/local/lib/libLLVMCore.a
build/deoptgen: /usr/local/lib/libLLVMCoroutines.a
build/deoptgen: /usr/local/lib/libLLVMFileCheck.a
build/deoptgen: /usr/local/lib/libLLVMInstCombine.a
build/deoptgen: /usr/local/lib/libLLVMInstrumentation.a
build/deoptgen: /usr/local/lib/libLLVMipo.a
build/deoptgen: /usr/local/lib/libLLVMIRPrinter.a
build/deoptgen: /usr/local/lib/libLLVMIRReader.a
build/deoptgen: /usr/local/lib/libLLVMLibDriver.a
build/deoptgen: /usr/local/lib/libLLVMLinker.a
build/deoptgen: /usr/local/lib/libLLVMObject.a
build/deoptgen: /usr/local/lib/libLLVMOption.a
build/deoptgen: /usr/local/lib/libLLVMPasses.a
build/deoptgen: /usr/local/lib/libLLVMScalarOpts.a
build/deoptgen: /usr/local/lib/libLLVMSupport.a
build/deoptgen: /usr/local/lib/libLLVMSymbolize.a
build/deoptgen: /usr/local/lib/libLLVMTarget.a
build/deoptgen: /usr/local/lib/libLLVMTargetParser.a
build/deoptgen: /usr/local/lib/libLLVMTextAPI.a
build/deoptgen: /usr/local/lib/libLLVMTransformUtils.a
build/deoptgen: /usr/local/lib/libLLVMVectorize.a
build/deoptgen: build/libUnoptGenCore.a
build/deoptgen: /usr/local/lib/libLLVMCFGuard.a
build/deoptgen: /usr/local/lib/libLLVMCoroutines.a
build/deoptgen: /usr/local/lib/libLLVMipo.a
build/deoptgen: /usr/local/lib/libLLVMInstrumentation.a
build/deoptgen: /usr/local/lib/libLLVMLinker.a
build/deoptgen: /usr/local/lib/libLLVMFrontendOpenMP.a
build/deoptgen: /usr/local/lib/libLLVMFrontendOffloading.a
build/deoptgen: /usr/local/lib/libLLVMIRPrinter.a
build/deoptgen: /usr/local/lib/libLLVMVectorize.a
build/deoptgen: /usr/local/lib/libLLVMCodeGen.a
build/deoptgen: /usr/local/lib/libLLVMBitWriter.a
build/deoptgen: /usr/local/lib/libLLVMScalarOpts.a
build/deoptgen: /usr/local/lib/libLLVMAggressiveInstCombine.a
build/deoptgen: /usr/local/lib/libLLVMInstCombine.a
build/deoptgen: /usr/local/lib/libLLVMTarget.a
build/deoptgen: /usr/local/lib/libLLVMCodeGenTypes.a
build/deoptgen: /usr/local/lib/libLLVMHipStdPar.a
build/deoptgen: /usr/local/lib/libLLVMObjCARCOpts.a
build/deoptgen: /usr/local/lib/libLLVMTransformUtils.a
build/deoptgen: /usr/local/lib/libLLVMAnalysis.a
build/deoptgen: /usr/local/lib/libLLVMProfileData.a
build/deoptgen: /usr/local/lib/libLLVMSymbolize.a
build/deoptgen: /usr/local/lib/libLLVMDebugInfoPDB.a
build/deoptgen: /usr/local/lib/libLLVMDebugInfoMSF.a
build/deoptgen: /usr/local/lib/libLLVMDebugInfoBTF.a
build/deoptgen: /usr/local/lib/libLLVMDebugInfoDWARF.a
build/deoptgen: /usr/local/lib/libLLVMObject.a
build/deoptgen: /usr/local/lib/libLLVMIRReader.a
build/deoptgen: /usr/local/lib/libLLVMBitReader.a
build/deoptgen: /usr/local/lib/libLLVMAsmParser.a
build/deoptgen: /usr/local/lib/libLLVMTextAPI.a
build/deoptgen: /usr/local/lib/libLLVMMCParser.a
build/deoptgen: /usr/local/lib/libLLVMMC.a
build/deoptgen: /usr/local/lib/libLLVMDebugInfoCodeView.a
build/deoptgen: /usr/local/lib/libLLVMCore.a
build/deoptgen: /usr/local/lib/libLLVMBinaryFormat.a
build/deoptgen: /usr/local/lib/libLLVMRemarks.a
build/deoptgen: /usr/local/lib/libLLVMBitstreamReader.a
build/deoptgen: /usr/local/lib/libLLVMTargetParser.a
build/deoptgen: /usr/local/lib/libLLVMSupport.a
build/deoptgen: /usr/local/lib/libLLVMDemangle.a
build/deoptgen: /usr/lib/x86_64-linux-gnu/libz.so
build/deoptgen: /usr/lib/x86_64-linux-gnu/libtinfo.so
build/deoptgen: src/tools/deoptgen/CMakeFiles/deoptgen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xchy/Projects/de3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../../build/deoptgen"
	cd /home/xchy/Projects/de3/src/tools/deoptgen && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/deoptgen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/tools/deoptgen/CMakeFiles/deoptgen.dir/build: build/deoptgen
.PHONY : src/tools/deoptgen/CMakeFiles/deoptgen.dir/build

src/tools/deoptgen/CMakeFiles/deoptgen.dir/clean:
	cd /home/xchy/Projects/de3/src/tools/deoptgen && $(CMAKE_COMMAND) -P CMakeFiles/deoptgen.dir/cmake_clean.cmake
.PHONY : src/tools/deoptgen/CMakeFiles/deoptgen.dir/clean

src/tools/deoptgen/CMakeFiles/deoptgen.dir/depend:
	cd /home/xchy/Projects/de3 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xchy/Projects/de3 /home/xchy/Projects/de3/src/tools/deoptgen /home/xchy/Projects/de3 /home/xchy/Projects/de3/src/tools/deoptgen /home/xchy/Projects/de3/src/tools/deoptgen/CMakeFiles/deoptgen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/tools/deoptgen/CMakeFiles/deoptgen.dir/depend
