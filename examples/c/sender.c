// Standard includes
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// External includes
#include "../../include/iipc.h"
#include "macro.h"

int main() {
    char* error = NULL;

    const char* channel_path = "example";
    iipc_channel_t* channel = iipc_get_channel(channel_path, &error);
    ASSERT_SUCCESS();

    unsigned long message_length = 10;
    char* message = (char*)malloc(message_length + 1);
    message[message_length] = '\0';

    for (char chr = 32; chr < 127; ++chr) {
        for (unsigned long idx = 0; idx < message_length; ++idx) {
            message[idx] = chr;
        }

        printf("sending: '%s'", message);
        iipc_channel_send(channel, message, &error);
        ASSERT_SUCCESS();

        usleep(200000);
    }

    free(message);

    iipc_channel_free(channel);
}
