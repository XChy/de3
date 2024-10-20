#include "DiffFeature.h"

using namespace llvm;

class InstFeature : public DiffFeature {
public:
  bool PutDiff(Function &L, Function &R, std::ostream &Out);
};
