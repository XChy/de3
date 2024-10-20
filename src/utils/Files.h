#pragma once
#include <string>
#include <vector>

std::vector<std::string> ReadPipeline(const std::string &Path);

int WritePipeline(const std::string &Path, const std::vector<std::string> &Pipeline);

// Read seed from path if the file exists, otherwise return time
ulong ReadSeed(const std::string &Path);
int WriteSeed(const std::string &Path, ulong Seed);
