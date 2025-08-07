// External includes
#include <gtest/gtest.h>

// Local includes
#include "../include/iipc.hpp"

TEST(channel_test, channel_get_path) {
    std::filesystem::path path = "test_channel_get_path_file";

    auto channel = iipc::get_channel(path);
    ASSERT_TRUE(channel.has_value()) << channel.error();

    auto channel_abs_path = channel->get_path();
    ASSERT_STREQ(std::filesystem::absolute(path).string().c_str(),
      channel_abs_path.string().c_str());
}
