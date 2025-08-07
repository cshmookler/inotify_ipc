// Local includes
#include "../build/version.h"

extern "C" {

const char* iipc_get_runtime_version() {
    return iipc_compiletime_version;
}

} // extern "C"
