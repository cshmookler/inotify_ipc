#ifndef IIPC_MACRO_H
#define IIPC_MACRO_H

#define ASSERT_SUCCESS()                                                       \
    if (error != NULL) {                                                       \
        int print_result = fprintf(stderr, "%s", error);                       \
        iipc_string_free(error);                                               \
        error = NULL;                                                          \
        if (print_result < 0) {                                                \
            return 2;                                                          \
        }                                                                      \
        return 1;                                                              \
    }

#endif // IIPC_MACRO_H
