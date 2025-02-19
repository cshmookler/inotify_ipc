// External includes
#include <gtest/gtest.h>

// Local includes
#include "../include/ipc.hpp"

TEST(channel_test, make_channel) {
    std::filesystem::path path = "test_make_channel_file";

    auto channel = iipc::make_channel(path);
}

TEST(channel_test, channel_get_path) {
    std::filesystem::path path = "test_channel_get_path_file";

    auto channel = iipc::make_channel(path);

    auto channel_abs_path = channel->get_path();
    ASSERT_STREQ(std::filesystem::absolute(path).string().c_str(),
      channel_abs_path.string().c_str());
}

TEST(channel_test, channel_set_path) {
    std::filesystem::path path_1 = "test_channel_set_path_file_1";
    std::filesystem::path path_2 = "test_channel_set_path_file_2";

    auto channel = iipc::make_channel(path_1);

    auto channel_abs_path = channel->get_path();
    ASSERT_STREQ(std::filesystem::absolute(path_1).string().c_str(),
      channel_abs_path.string().c_str());

    auto result = channel->set_path(path_2);
    ASSERT_TRUE(result.success());

    channel_abs_path = channel->get_path();
    ASSERT_STREQ(std::filesystem::absolute(path_2).string().c_str(),
      channel_abs_path.string().c_str());
}

TEST(channel_test, channel_send_receive) {
    std::filesystem::path path = "test_channel_send_file";

    auto channel = iipc::make_channel(path);

    std::string message = "this is a test";
    auto result = channel->send(message);
    ASSERT_TRUE(result.success());

    auto poll_status = channel->poll();
    ASSERT_TRUE(poll_status.has_value());
    ASSERT_TRUE(poll_status.value());

    auto received_message = channel->receive();
    ASSERT_TRUE(received_message.has_value());

    ASSERT_STREQ(message.c_str(), received_message->c_str());
}
