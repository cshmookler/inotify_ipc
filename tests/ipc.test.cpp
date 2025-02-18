// External includes
#include <gtest/gtest.h>

// Local includes
#include "../include/ipc.hpp"

TEST(channel_test, make_channel) {
    std::filesystem::path path = "test_file";
    auto channel = iipc::make_channel(path);
}
