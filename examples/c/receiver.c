// Standard includes
#include <stdio.h>
#include <stdlib.h>

// External includes
#include "../../include/iipc.h"
#include "macro.h"

int main() {
    char* error = NULL;

    const char* channel_path = "example";
    iipc_channel_t* channel = iipc_get_channel(channel_path, &error);
    ASSERT_SUCCESS();

    const unsigned long timeout_ms = 10000;

    while (1) {
        int has_value = iipc_channel_poll(channel, timeout_ms, &error);
        ASSERT_SUCCESS();
        if (has_value) {
            char* message = iipc_channel_receive(channel, &error);
            ASSERT_SUCCESS();
            printf("received: '%s'", message);
            free(message);
        }
    }

    iipc_channel_free(channel);
}
