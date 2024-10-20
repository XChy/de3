file(GLOB_RECURSE MOCLASSIFY_SRC "*.h"
     "*.cpp")

add_executable(moclassify main.cpp ${MOCLASSIFY_SRC})

target_link_libraries(moclassify UnoptGenCore ${llvm_libs})
