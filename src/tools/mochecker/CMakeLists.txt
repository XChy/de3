file(GLOB_RECURSE MOCHECKER_SRC "*.h"
     "*.cpp")

add_executable(mochecker main.cpp ${MOCHECKER_SRC})

target_link_libraries(mochecker UnoptGenCore ${llvm_libs})
