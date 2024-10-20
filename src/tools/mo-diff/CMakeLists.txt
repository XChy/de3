file(GLOB_RECURSE MODIFF_SRC "lib/*.h"
"lib/*.cpp")

add_executable(mo-diff ${MODIFF_SRC} main.cpp)

target_link_libraries(mo-diff ${llvm_libs} UnoptGenCore)
