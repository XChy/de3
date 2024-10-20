#include "tools/deoptgen/engine/Mutator.h"
#include "utils/Debug.h"
#include "utils/Files.h"
#include "utils/Random.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/WithColor.h"
#include <cstdlib>
#include <format>
#include <iostream>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/CommandLine.h>
#include <memory>
#include <system_error>

using namespace llvm;
using std::format;

cl::OptionCategory UnoptGenOptions("UnoptGen Options");

static cl::opt<std::string> InputFile(cl::Positional,
                                      cl::desc("<File to mutate>"),
                                      cl::Required, cl::cat(UnoptGenOptions));

static cl::opt<std::string> OutputFile("o", cl::desc("File to output"),
                                       cl::cat(UnoptGenOptions), cl::init(""));

static cl::opt<std::string> SeedFile("s", cl::desc("Seed file"),
                                     cl::cat(UnoptGenOptions), cl::init(""));

static cl::opt<int> MaxPasses("m", cl::desc("Max number of passes to run"),
                              cl::cat(UnoptGenOptions), cl::init(5));

static cl::opt<std::string> PipelineFile("p", cl::desc("File of pipeline list"),
                                         cl::cat(UnoptGenOptions),
                                         cl::init(""));

static cl::opt<std::string> TraceDir("trace", cl::desc("Directory of traces"),
                                     cl::cat(UnoptGenOptions), cl::init(""));

void mutate(Module &M);

static std::unique_ptr<Module> readModule(LLVMContext &Context,
                                          StringRef Name) {
  SMDiagnostic Diag;
  std::unique_ptr<Module> M = parseIRFile(Name, Diag, Context);
  if (!M)
    Diag.print("momutate", errs());
  return M;
}

static void writeModule(Module &M, StringRef Name) {
  // TODO: Support bitcode writer
  std::error_code EC;
  raw_fd_ostream Out(Name, EC, sys::fs::OF_Text);
  M.print(Out, nullptr);
}

int main(int Argc, char **Argv) {
  cl::HideUnrelatedOptions({&UnoptGenOptions, &getColorCategory()});
  cl::ParseCommandLineOptions(Argc, Argv);

  ulong Seed = ReadSeed(SeedFile);
  InstallSeed(Seed);
  WriteSeed(SeedFile, Seed);

  // Read IR
  LLVMContext Context;
  std::unique_ptr<Module> Module = readModule(Context, InputFile);
  if (Module)
    MODEBUG(dbgs() << ::format("Mutating IR: {}\n", InputFile.getValue()));
  else {
    errs() << ::format("Broken IR: {}\n", InputFile.getValue());
    return -1;
  }

  // Mutate
  Mutator mutator(MaxPasses, PipelineFile, TraceDir);
  mutator.generateOrReadPipeline();
  mutator.storeConfig();

  mutator.mutate(*Module);

  if (OutputFile.empty())
    Module->print(outs(), nullptr);
  else
    writeModule(*Module, OutputFile);
}
