#include "ExprExpandPass.h"
#include "transforms/ExprExpand/ExprExpander.h"

using namespace llvm;

void ExprExpandPass::printPipeline(
    raw_ostream &OS, function_ref<StringRef(StringRef)> MapClassName2PassName) {
}

PreservedAnalyses ExprExpandPass::run(Function &F,
                                      FunctionAnalysisManager &AM) {
  IRBuilder<> Builder(F.getContext());

  ExprExpander(Builder).run(F);

  PreservedAnalyses PA;
  PA.preserveSet<CFGAnalyses>();
  return PA;
}
