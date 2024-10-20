#include "Random.h"

ulong RandomSeed;
std::mt19937 Gen;

void InstallSeed(ulong Seed) {
  RandomSeed = Seed;
  Gen = std::mt19937(Seed);
}

uint choose(uint n) {
  std::uniform_int_distribution<> Distribute(0, n - 1);
  return Distribute(Gen);
}
