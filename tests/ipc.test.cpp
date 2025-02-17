// External includes
#include <gtest/gtest.h>

// Local includes
#include "../include/ipc.hpp"

TEST(channel_test, make_channel) {
    auto channel = iipc::make_channel("test_file");
}
