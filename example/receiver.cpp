// Standard includes
#include <iostream>

// External includes
#include "../include/ipc.hpp"

int main() {
    auto channel = iipc::make_channel("example");
    if (channel.has_error()) {
        std::cout << channel.error().string() << '\n';
        return 1;
    }

    while (true) {
        auto poll = channel->poll(std::chrono::milliseconds(10000));
        if (poll.has_error()) {
            std::cout << poll.error().string() << '\n';
            return 1;
        }
        if (! poll.value()) {
            std::cout << "Timed out" << '\n';
            return 1;
        }

        auto message = channel->receive();
        if (message.has_error()) {
            std::cout << message.error().string() << '\n';
            return 1;
        }

        std::cout << "received: '" << message.value() << "'\n";
    }
}
