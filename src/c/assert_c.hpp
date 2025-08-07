#pragma once

#define ASSERT_NOT_NULL(arg, return_val)                                       \
    if (arg == NULL) {                                                         \
        if (error != NULL) {                                                   \
            *error = strdup(                                                   \
              RES_NEW_ERROR("A required argument is null.").string().c_str()); \
        }                                                                      \
        return return_val;                                                     \
    }

#define ASSERT_SUCCESS(call, return_val)                                       \
    auto result = call;                                                        \
    if (result.failure()) {                                                    \
        if (error != NULL) {                                                   \
            *error = strdup(result.error().string().c_str());                  \
        }                                                                      \
        return return_val;                                                     \
    }

#define ASSERT_HAS_VALUE(result, return_val)                                   \
    if (result.has_error()) {                                                  \
        if (error != NULL) {                                                   \
            *error = strdup(result.error().string().c_str());                  \
        }                                                                      \
        return return_val;                                                     \
    }
