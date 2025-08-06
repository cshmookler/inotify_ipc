// External includes
#include <gtest/gtest.h>

// Local includes
#include "../include/ipc.hpp"

TEST(channel_test, channel_send_receive) {
    std::filesystem::path path = "test_channel_send_file";

    auto channel = iipc::get_channel(path);
    ASSERT_TRUE(channel.has_value()) << channel.error();

    std::string message = "this is a test";
    auto result = channel->send(message);
    ASSERT_TRUE(result.success()) << result.error();

    auto poll_status = channel->poll();
    ASSERT_TRUE(poll_status.has_value()) << poll_status.error();
    ASSERT_TRUE(poll_status.value());

    auto received_message = channel->receive();
    ASSERT_TRUE(received_message.has_value()) << received_message.error();

    ASSERT_STREQ(message.c_str(), received_message->c_str());
}
