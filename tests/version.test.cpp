// External includes
#include <gtest/gtest.h>

// Local includes
#include "../build/version.h"

TEST(version_test, runtime_version_matches_compiletime_version) {
    ASSERT_STREQ(iipc_get_runtime_version(), iipc_compiletime_version);
}
