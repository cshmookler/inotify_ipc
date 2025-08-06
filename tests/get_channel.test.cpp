// External includes
#include <gtest/gtest.h>

// Local includes
#include "../include/ipc.hpp"

TEST(channel_test, get_channel) {
    std::filesystem::path path = "test_get_channel_file";

    auto channel = iipc::get_channel(path);
    ASSERT_TRUE(channel.has_value()) << channel.error();
}
