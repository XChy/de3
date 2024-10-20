#pragma once

#include <llvm/IR/Module.h>

namespace llvm {

/*
 * Control the main mutating procedure, managing all
 * semantics-preserving passes.
 */

class Mutator {
public:
  Mutator(int MaxPassesNum, const std::string &PipelineFile,
          const std::string &TraceDir)
      : MaxPassesNum(MaxPassesNum), PipelineFile(PipelineFile),
        TraceDir(TraceDir) {}

  // Mutate M with a pipeline of semantics-preserving passes (randomized or
  // determined). Return 0 if succeeding, otherwise return -1.
  int mutate(Module &M);
  void storeConfig();

  void generateOrReadPipeline();

private:
  int MaxPassesNum;
  std::string PipelineFile;
  std::string TraceDir;
  std::vector<std::string> Pipeline;
};

} // namespace llvm
