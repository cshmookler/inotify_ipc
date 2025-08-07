// Standard includes
#include <cstring>

// Local includes
#include "../../include/iipc.h"

extern "C" {

void iipc_string_free(char* error) {
    free(error);
}

} // extern "C"
