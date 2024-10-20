#include "indicators/DiffChecker.h"
#include "indicators/Indicator.h"
#include "indicators/InlineIndicator.h"
#include "indicators/InstCountIndicator.h"
#include "indicators/StaticProfileIndicator.h"
#include "indicators/UBChecker.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/WithColor.h"
#include <iostream>
#include <llvm/ADT/StringExtras.h>
#include <llvm/ADT/iterator_range.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/CommandLine.h>

using namespace llvm;

cl::OptionCategory MOClassifyOptions("MOClassify Options");

static cl::opt<std::string> LeftFilename(cl::Positional,
                                         cl::desc("<first file>"), cl::Required,
                                         cl::cat(MOClassifyOptions));

static cl::opt<std::string> RightFilename(cl::Positional,
                                          cl::desc("<second file>"),
                                          cl::Required,
                                          cl::cat(MOClassifyOptions));

static cl::opt<std::string> FuncName("func", cl::desc("<Function Name>"),
                                     cl::init("."), cl::cat(MOClassifyOptions));

static cl::opt<bool> OnlyCheckUB("only-ub",
                                 cl::desc("<only check undefined behaviour>"),
                                 cl::cat(MOClassifyOptions));

static cl::opt<bool> ReverseCheck("reverse", cl::desc("<check reversely>"),
                                  cl::cat(MOClassifyOptions), cl::init(false));

static std::unique_ptr<Module> readModule(LLVMContext &Context,
                                          StringRef Name) {
  SMDiagnostic Diag;
  std::unique_ptr<Module> M = parseIRFile(Name, Diag, Context);
  if (!M)
    Diag.print("mochecker", errs());
  return M;
}

int main(int Argc, char **Argv) {
  cl::HideUnrelatedOptions({&MOClassifyOptions, &getColorCategory()});
  cl::ParseCommandLineOptions(Argc, Argv);

  LLVMContext Context;

  std::unique_ptr<Module> LModule = readModule(Context, LeftFilename);
  std::unique_ptr<Module> RModule = readModule(Context, RightFilename);
  if (!LModule || !RModule)
    return 1;

  std::vector<std::shared_ptr<Indicator>> Indicators;
  if (OnlyCheckUB)
    Indicators = {std::make_shared<UBChecker>()};
  else
    Indicators = {
        std::make_shared<InstCountIndicator>(),
        std::make_shared<UBChecker>(),
        std::make_shared<InlineIndicator>(),
        std::make_shared<StaticProfileIndicator>(),
        std::make_shared<DiffChecker>(),
    };

  auto IsBetter = [&](Function &L, Function &R) -> bool {
    return std::all_of(
        Indicators.begin(), Indicators.end(),
        [&](std::shared_ptr<Indicator> I) { return I->worth(L, R) > 0; });
  };

  Function *LF = LModule->getFunction(FuncName);
  Function *RF = RModule->getFunction(FuncName);

  if (!LF || !RF)
    return -1;

  bool Success;
  if (!ReverseCheck)
    Success = IsBetter(*LF, *RF);
  else
    Success = IsBetter(*RF, *LF);

  if (Success)
    std::cout << "OK\n";

  return 0;
}
