// Standard includes
#include <stdio.h>

// External includes
#include "../build/version.h"

int main() {
    printf("Compile-time version: %s\n", iipc_compiletime_version);
    printf("Runtime version: %s\n", iipc_get_runtime_version());
}
