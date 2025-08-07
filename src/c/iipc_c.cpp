// Standard includes
#include <cstring>

// Local includes
#include "../../include/iipc.h"
#include "assert_c.hpp"

extern "C" {

iipc_channel_t* iipc_get_channel(const char* path, char** error) {
    ASSERT_NOT_NULL(path, NULL);
    auto result = iipc::get_channel(path);
    ASSERT_HAS_VALUE(result, NULL);
    return result.release();
}

void iipc_channel_free(iipc_channel_t* channel) {
    delete channel;
}

char* iipc_channel_get_path(iipc_channel_t* channel, char** error) {
    ASSERT_NOT_NULL(channel, NULL);
    auto result = channel->get_path();
    return strdup(result.c_str());
}

void iipc_channel_set_path(
  iipc_channel_t* channel, const char* path, char** error) {
    ASSERT_NOT_NULL(channel, );
    ASSERT_NOT_NULL(path, );
    ASSERT_SUCCESS(channel->set_path(path), );
}

void iipc_channel_send(
  iipc_channel_t* channel, const char* message, char** error) {
    ASSERT_NOT_NULL(channel, );
    ASSERT_SUCCESS(channel->send(message), );
}

int iipc_channel_poll(
  iipc_channel_t* channel, unsigned long timeout_ms, char** error) {
    ASSERT_NOT_NULL(channel, 0);
    auto result = channel->poll(std::chrono::milliseconds(timeout_ms));
    ASSERT_HAS_VALUE(result, 0);
    return static_cast<int>(result.value());
}

char* iipc_channel_receive(iipc_channel_t* channel, char** error) {
    ASSERT_NOT_NULL(channel, NULL);
    auto result = channel->receive();
    ASSERT_HAS_VALUE(result, NULL);
    return strdup(result.value().c_str());
}

} // extern "C"
