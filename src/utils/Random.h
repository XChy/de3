#pragma once
#include <inttypes.h>
#include <random>
#include <stdlib.h>
#include <vector>

void InstallSeed(ulong Seed);

// Use our own random number generator, to avoid interruption from some library
// calls
uint choose(uint n);
static inline bool whether() { return choose(2); }

template <typename Exec> class RandomExecutor {
public:
  void add(Exec &exec) { executors.push_back(exec); }

  // Return the index of the executed one
  template <typename... Args> uint execute(Args &&...args) {
    uint chosen = choose(executors.size());
    executors[chosen](args...);
    return chosen;
  }

  template <typename... Args> void execute(uint index, Args &&...args) {
    executors[index](args...);
  }

private:
  std::vector<Exec> executors;
};
