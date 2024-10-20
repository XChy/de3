#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Debug.h>

namespace llvm {

cl::opt<bool> MODebug("modebug", cl::desc("open debug mode in modetect"),
                      cl::init(false));

} // namespace llvm
