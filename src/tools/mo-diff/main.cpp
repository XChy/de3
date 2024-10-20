#include "lib/InstFeature.h"
#include "tools/mo-diff/lib/DiffFeature.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/WithColor.h"
#include <fstream>
#include <iostream>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/CommandLine.h>

using namespace llvm;

cl::OptionCategory MODiffOptions("MODiff Options");

static cl::opt<std::string> LeftFile(cl::Positional, cl::desc("<first file>"),
                                     cl::Required, cl::cat(MODiffOptions));
static cl::opt<std::string> RightFile(cl::Positional, cl::desc("<second file>"),
                                      cl::Required, cl::cat(MODiffOptions));
static cl::opt<std::string> DiffFile("o", cl::desc("<diff file to output>"),
                                     cl::cat(MODiffOptions));
static cl::opt<std::string> FuncName("func", cl::desc("<function name>"),
                                     cl::cat(MODiffOptions));

static std::unique_ptr<Module> readModule(LLVMContext &Context,
                                          StringRef Name) {
  SMDiagnostic Diag;
  std::unique_ptr<Module> M = parseIRFile(Name, Diag, Context);
  if (!M)
    Diag.print("mo-diff", errs());
  return M;
}

static void writeModule(Module &M, StringRef Name) {
  // TODO: Support bitcode writer
  std::error_code EC;
  raw_fd_ostream Out(Name, EC, sys::fs::OF_Text);
  M.print(Out, nullptr);
}

int main(int Argc, char **Argv) {
  cl::HideUnrelatedOptions({&MODiffOptions, &getColorCategory()});
  cl::ParseCommandLineOptions(Argc, Argv);

  LLVMContext Context;
  InstFeature IF;
  std::vector<DiffFeature *> Features = {&IF};

  std::unique_ptr<Module> LModule = readModule(Context, LeftFile);
  std::unique_ptr<Module> RModule = readModule(Context, RightFile);
  if (!LModule || !RModule)
    return -1;

  std::ofstream FOut;
  std::ostream *Out;
  if (DiffFile.empty()) {
    Out = &std::cout;
  } else {
    Out = &FOut;
    FOut.open(DiffFile, std::ios::out);
    if (FOut.fail())
      return -1;
  }

  Function &L = *LModule->getFunction(FuncName);
  Function &R = *RModule->getFunction(FuncName);
  for (auto *Feature : Features)
    Feature->PutDiff(L, R, *Out);

  if (FOut.is_open())
    FOut.close();
}
