// External includes
#include <gtest/gtest.h>

// Local includes
#include "../include/iipc.hpp"

TEST(channel_test, channel_set_path) {
    std::filesystem::path path_1 = "test_channel_set_path_file_1";
    std::filesystem::path path_2 = "test_channel_set_path_file_2";

    auto channel = iipc::get_channel(path_1);
    ASSERT_TRUE(channel.has_value()) << channel.error();

    auto channel_abs_path = channel->get_path();
    ASSERT_STREQ(std::filesystem::absolute(path_1).string().c_str(),
      channel_abs_path.string().c_str());

    auto result = channel->set_path(path_2);
    ASSERT_TRUE(result.success()) << result.error();

    channel_abs_path = channel->get_path();
    ASSERT_STREQ(std::filesystem::absolute(path_2).string().c_str(),
      channel_abs_path.string().c_str());
}
