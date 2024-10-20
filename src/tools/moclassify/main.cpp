#include "indicators/DiffChecker.h"
#include "indicators/Indicator.h"
#include "indicators/InlineIndicator.h"
#include "indicators/InstCountIndicator.h"
#include "indicators/LoopIndicator.h"
#include "indicators/StaticProfileIndicator.h"
#include "indicators/UBChecker.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/WithColor.h"
#include <filesystem>
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

static cl::opt<std::string> OutputDir("output", cl::desc("<Output directory>"),
                                      cl::init("."),
                                      cl::cat(MOClassifyOptions));

static std::unique_ptr<Module> readModule(LLVMContext &Context,
                                          StringRef Name) {
  SMDiagnostic Diag;
  std::unique_ptr<Module> M = parseIRFile(Name, Diag, Context);
  if (!M)
    Diag.print("moclassify", errs());
  return M;
}

template <typename T> static std::string joinStringList(T Set) {
  return llvm::join(llvm::make_range(Set.begin(), Set.end()), ",");
}

static void writeModule(Module &M, StringRef Name) {
  // TODO: Support bitcode writer
  std::error_code EC;
  raw_fd_ostream Out(Name, EC, sys::fs::OF_Text);
  M.print(Out, nullptr);
}

static void extractFunc(const std::string &ModuleFile,
                        const std::string &FuncName,
                        const std::filesystem::path &OutputPath) {
  system(std::format("mkdir -p {}", OutputPath.parent_path().string()).data());
  system(std::format("llvm-extract {} -S --func=\"{}\" -o {}", ModuleFile,
                     FuncName, OutputPath.string())
             .data());
}

int main(int Argc, char **Argv) {
  cl::HideUnrelatedOptions({&MOClassifyOptions, &getColorCategory()});
  cl::ParseCommandLineOptions(Argc, Argv);

  LLVMContext Context;

  std::unique_ptr<Module> LModule = readModule(Context, LeftFilename);
  std::unique_ptr<Module> RModule = readModule(Context, RightFilename);
  if (!LModule || !RModule)
    return 1;

  std::vector<std::shared_ptr<Indicator>> Indicators = {
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

  std::filesystem::path OutputDirPath(OutputDir.getValue());

  uint32_t Index = 0;
  bool InterestingNess = false;
  for (Function &LF : *LModule) {
    if (LF.isDeclaration())
      continue;
    Function *RF = RModule->getFunction(LF.getName());
    if (!RF || RF->isDeclaration())
      continue;

    std::string FuncName = LF.getName().data();

    Index++;
    auto IndexStr = std::to_string(Index);
    // LF is better than RF
    if (IsBetter(LF, *RF)) {
      InterestingNess = true;
      std::filesystem::path Dir = OutputDirPath / "better" / IndexStr;
      extractFunc(LeftFilename, FuncName, (Dir / "mutated.ll"));
      extractFunc(RightFilename, FuncName, (Dir / "original.ll"));
      system(std::format("echo -n \"{}\" > {}", FuncName,
                         (Dir / "func_name").string())
                 .c_str());
    } else {
      // TODO: How do we handle not that good cases?
      continue;
    }
  }

  return !InterestingNess;
}
