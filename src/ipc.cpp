// Standard includes
#include <cstring>
#include <fstream>
#include <vector>

// External includes
#include <sys/inotify.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Local includes
#include "../include/ipc.hpp"

namespace iipc {

struct channel_t::global_state_t {
    int inotify_fd;
};

std::shared_ptr<channel_t::global_state_t> channel_t::global_state;

struct channel_t::local_state_t {
    std::filesystem::path path;
    int watcher_fd;
};

[[nodiscard]] res::optional_t<channel_t> make_channel(
  const std::filesystem::path& path) {
    if (! std::filesystem::exists(path)) {
        // Attempt to create the channel file if it doesn't exist.
        std::ofstream file{ path };
        if (! file.is_open()) {
            return RES_NEW_ERROR("Failed to create the channel file.\n\tfile: '"
              + path.string() + "'");
        }
    }
    if (! std::filesystem::is_regular_file(path)) {
        return RES_NEW_ERROR(
          "The channel path exists but is not a regular file.\n\tpath: '"
          + path.string() + "'");
    }

    if (channel_t::global_state == nullptr) {
        int inotify_fd = inotify_init();
        if (inotify_fd < 0) {
            return RES_NEW_ERROR("Failed to initialize inotify.");
        }

        channel_t::global_state_t global_state{};
        global_state.inotify_fd = inotify_fd;

        channel_t::global_state =
          std::make_shared<channel_t::global_state_t>(global_state);
    }

    // Events to generate notifications for.
    const auto event_mask = IN_CLOSE_WRITE;

    int watcher_fd = inotify_add_watch(
      channel_t::global_state->inotify_fd, path.string().c_str(), event_mask);
    if (watcher_fd < 0) {
        return RES_NEW_ERROR(
          "Failed to create a new watch on a file.\n\tfile: '" + path.string()
          + "'\n\tinotify event mask: '" + std::to_string(event_mask) + "'");
    }

    channel_t::local_state_t local_state{};
    local_state.path = path;
    local_state.watcher_fd = watcher_fd;

    return channel_t{ local_state };
}

channel_t::channel_t(const local_state_t& local_state)
: global_state_(channel_t::global_state)
, local_state_(std::make_unique<local_state_t>(local_state)) {
}

channel_t::channel_t(channel_t&& channel) noexcept
: global_state_(std::move(channel.global_state_))
, local_state_(std::move(channel.local_state_)) {
}

channel_t& channel_t::operator=(channel_t&& channel) noexcept {
    this->local_state_ = std::move(channel.local_state_);

    return *this;
}

channel_t::~channel_t() {
    if (this->global_state_ == nullptr) {
        // Global state is always initialized unless this object has been moved.
        // Therefore, if global state is not initialized, then destruction must
        // be skipped because all objects have been moved and are currently
        // invalid.
        return;
    }

    if (inotify_rm_watch(
          channel_t::global_state_->inotify_fd, this->local_state_->watcher_fd)
      < 0) {
        // Assume destruction always succeeds.
    }

    if (channel_t::global_state.use_count() == 2) {
        // Attempt to uninitialize inotify when there aren't any open channels.
        if (close(channel_t::global_state->inotify_fd) < 0) {
            // Assume destruction always succeeds.
        }
    }
}

std::filesystem::path channel_t::get_path() const {
    return this->local_state_->path;
}

void channel_t::set_path(const std::filesystem::path& path) {
    this->local_state_->path = path;
}

res::result_t channel_t::send(const std::string& message) const {
    std::ofstream file{ this->local_state_->path };
    if (file.fail()) {
        return RES_NEW_ERROR("Failed to open a file for writing.\n\tfile: '"
          + this->local_state_->path.string() + "'");
    }

    if ((file << message).fail()) {
        return RES_NEW_ERROR("Failed to write a message to a file.\n\tfile: '"
          + this->local_state_->path.string() + "'\n\tmessage: '" + message
          + "'");
    }

    return res::success;
}

res::optional_t<bool> channel_t::poll(std::chrono::milliseconds timeout) const {
    struct pollfd poll_result{};
    poll_result.fd = channel_t::global_state_->inotify_fd;
    poll_result.events = POLLIN;

    // Wait for an event until the timeout expires.
    int poll_status =
      ::poll(&poll_result, 1, static_cast<int>(timeout.count()));
    if (poll_status < 0) {
        if (errno == EINTR) {
            // Interrupts should be handled the same as an expired timeout.
            return false;
        }
        return RES_NEW_ERROR("Encountered an error while listening for new "
                             "messages from a file.\n\tfile: '"
          + this->local_state_->path.string() + "'");
    }
    if (poll_status == 0) {
        // The timeout expired.
        return false;
    }

    // Read all available events to reset event polling.

    int event_size = 0;
    if (ioctl(channel_t::global_state_->inotify_fd, FIONREAD, &event_size)
      < 0) {
        return RES_NEW_ERROR("Failed to retrieve the number of events.");
    }

    std::vector<char> event_buffer;
    event_buffer.reserve(event_size);
    if (read(
          channel_t::global_state_->inotify_fd, event_buffer.data(), event_size)
      < 0) {
        return RES_NEW_ERROR("Failed to read events.");
    }

    // The event data is currently unused.

    return true;
}

res::optional_t<std::string> channel_t::receive() const {
    std::ifstream file{ this->local_state_->path, std::ifstream::ate };
    if (file.fail()) {
        return RES_NEW_ERROR("Failed to open a file for reading.\n\tfile: '"
          + this->local_state_->path.string() + "'");
    }

    auto file_size = file.tellg();
    if (file_size == -1) {
        return RES_NEW_ERROR("Failed to get the size of a file.\n\tfile: '"
          + this->local_state_->path.string() + "'");
    }

    if (file.seekg(0).fail()) {
        return RES_NEW_ERROR("Failed to set the input position to the "
                             "beginning of a file.\n\tfile: '"
          + this->local_state_->path.string() + "'");
    }

    std::string message(file_size, '\0');

    if ((file.read(message.data(), file_size)).fail()) {
        const char* error = std::strerror(errno);
        return RES_NEW_ERROR("Failed to read a message from a file.\n\tfile: '"
          + this->local_state_->path.string() + "'\n\terror: '" + error + "'");
    }

    return message;
}

} // namespace iipc
