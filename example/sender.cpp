// Standard includes
#include <iostream>
#include <thread>

// External includes
#include "../include/ipc.hpp"

int main() {
    auto channel = iipc::make_channel("example");
    if (channel.has_error()) {
        std::cout << channel.error().string() << '\n';
        return 1;
    }

    for (u_char chr = 32; chr < 127; chr++) {
        std::string message(10, static_cast<char>(chr));
        std::cout << "sending: '" << message << "'\n";
        auto result = channel->send(message);
        if (result.failure()) {
            std::cout << result.error().string() << '\n';
            return 1;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
