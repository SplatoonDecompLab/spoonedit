#include"NativeAssembly.h"

#ifdef __unix
#   include"Unix/NativeAssembly.h"
#elif _WIN64
#   include"Windows/NativeAssembly.h"
#endif
