#pragma once

/*****************************************************************************/
/*  Copyright (c) 2025 Caden Shmookler                                       */
/*                                                                           */
/*  This software is provided 'as-is', without any express or implied        */
/*  warranty. In no event will the authors be held liable for any damages    */
/*  arising from the use of this software.                                   */
/*                                                                           */
/*  Permission is granted to anyone to use this software for any purpose,    */
/*  including commercial applications, and to alter it and redistribute it   */
/*  freely, subject to the following restrictions:                           */
/*                                                                           */
/*  1. The origin of this software must not be misrepresented; you must not  */
/*     claim that you wrote the original software. If you use this software  */
/*     in a product, an acknowledgment in the product documentation would    */
/*     be appreciated but is not required.                                   */
/*  2. Altered source versions must be plainly marked as such, and must not  */
/*     be misrepresented as being the original software.                     */
/*  3. This notice may not be removed or altered from any source             */
/*     distribution.                                                         */
/*****************************************************************************/

/**
 * @file notify.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Utilities for notifying the status bar that it is out-of-date.
 * @date 2024-01-19
 */

// Standard includes
#include <chrono>
#include <filesystem>
#include <memory>

// External includes
#include <cpp_result/all.hpp>

namespace iipc {

class channel_t;

[[nodiscard]] res::optional_t<channel_t> make_channel(
  const std::filesystem::path& path);

/**
 * @brief Represents an individual connection channel.  Two programs must have
 * their channel objects bound to the same path to communicate.
 */
class channel_t {
    // Shared state for all instances of this class.
    struct global_state_t;
    static std::shared_ptr<global_state_t> global_state;
    std::shared_ptr<global_state_t> global_state_;

    // Local state for individual instances of this class.
    struct local_state_t;
    std::unique_ptr<local_state_t> local_state_;

    // Some functions require access to private members.
    friend res::optional_t<channel_t> make_channel(
      const std::filesystem::path&);

    // Constructors
    channel_t(const local_state_t& local_state);
    channel_t(const channel_t&) = delete;

  public:
    // Move constructors are public because, unlike copy constructors, the total
    // number of class instances does not change.
    channel_t(channel_t&& channel) noexcept;

    // Operators
    channel_t& operator=(const channel_t&) = delete;
    channel_t& operator=(channel_t&& channel) noexcept;

    // Destructor
    ~channel_t();

    /**
     * @return the file path corresponding to this channel.
     */
    [[nodiscard]] std::filesystem::path get_path() const;

    /**
     * @brief Set the path to the file corresponding to this channel.
     *
     * @param[in] path - The file used when reading and writing messages.
     */
    void set_path(const std::filesystem::path& path);

    /**
     * @brief Sends a message to other programs with the same channel path.
     *
     * Any program that is listening for changes and has permission to read from
     * the channel file can receive sent messages.
     *
     * @param[in] message - The message to send to other programs listenening
     * for changes to the channel file.
     * @return a result indicating success or failure.
     */
    [[nodiscard]] res::result_t send(const std::string& message) const;

    /**
     * @brief Wait for a new message to be sent or until the timeout expires.
     *
     * @return true if a message is ready to be received, false if the timeout
     * expired, and an error if the presence of a new message could not be
     * determined.
     */
    [[nodiscard]] res::optional_t<bool> poll(
      std::chrono::milliseconds timeout) const;

    /**
     * @brief Read a file to get the last sent message.
     *
     * @return the last message sent or an error if the message could not be
     * retrieved.
     */
    [[nodiscard]] res::optional_t<std::string> receive() const;
};

} // namespace iipc
