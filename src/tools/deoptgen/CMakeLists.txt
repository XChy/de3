add_executable(deoptgen main.cpp engine/Mutator.cpp)

target_link_libraries(deoptgen ${llvm_libs} UnoptGenCore)
