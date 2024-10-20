#include "Files.h"
#include <fstream>
#include <iostream>
#include <random>

std::vector<std::string> ReadPipeline(const std::string &Path) {
  std::vector<std::string> Ret;
  std::ifstream In;
  In.open(Path, std::ios::in);
  if (In.fail())
    return {};

  while (!In.eof()) {
    std::string Elt;
    In >> Elt;
    Ret.push_back(Elt);
  }
  Ret.erase(Ret.end());

  In.close();
  return Ret;
}

int WritePipeline(const std::string &Path,
                  const std::vector<std::string> &Pipeline) {
  std::vector<int> Ret;
  std::ofstream Out;
  Out.open(Path, std::ios::out);

  if (Out.fail())
    return -1;

  for (const auto &Elt : Pipeline)
    Out << Elt << "\n";

  Out.close();
  return 0;
}

ulong ReadSeed(const std::string &Path) {
  ulong Ret;
  std::ifstream In;
  In.open(Path, std::ios::in);
  if (In.fail()) {
    std::random_device RD;
    Ret = RD();
  } else
    In >> Ret;

  In.close();
  return Ret;
}

int WriteSeed(const std::string &Path, ulong Seed) {
  std::ofstream Out;
  Out.open(Path, std::ios::out);

  if (Out.fail())
    return -1;

  Out << Seed;
  Out.close();
  return 0;
}
