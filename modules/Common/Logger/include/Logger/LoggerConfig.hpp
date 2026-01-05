/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef LOGGER_LOGGERCONFIG_HPP
#define LOGGER_LOGGERCONFIG_HPP

#include <chrono>
#include <cstdint>
#include <expected>
#include <filesystem>
#include <string>

#include "LoggerLevel.hpp"

namespace medlog
{

/**
 * \struct LoggerConfig
 *
 * @brief Structure holding the configurable parameters to tune the behavior of the
 * logger. To be injected to the logger at initialization.
 */
struct LoggerConfig final
{
	std::string app_name = "app";
	std::filesystem::path log_dir = "logs";
	std::string log_filename = "app.log";
	std::string error_log_filename = "error.log";
	std::string user_event_name = "UserEvent";
	std::string user_event_log_filename = "UserEvent.log";

	// Rotation settings
	std::size_t max_file_size_megabytes = 50ULL;  // 50 MB
	std::size_t max_files = 10;

	// Async queue size (tradeoff between memory and drop risk).
	std::size_t async_queue_size = 8192;

	// Number of worker thread. A value of 1 preserves the message order after dequeuing
	std::size_t thread_count = 1;

	// Minimum level written.
	LogLevel level = LogLevel::Info;

	std::chrono::milliseconds flush_every{1000};  // Every second

	// Pattern: Date ISO8601, thread id, level, logger name, message
	std::string pattern = "[%Y-%m-%d %H:%M:%S.%e][%^%t%$][%-8l]%v";

	// Useful logs for audit trail
	bool enable_separate_error_log = false;
	bool enable_user_event_log = false;

	// Default constructor (uses default values)
	LoggerConfig() = default;
};

/**
 * @brief Generate a LoggerConfig object populated with the content of the configuration
 * file
 *
 * @param: filepath of the configuration file
 *
 * @return: Configuration object for the logs
 */
std::expected<LoggerConfig, std::string> loadConfigurationFile(
    const std::filesystem::path& filename);

}  // namespace medlog

#endif /* LOGGER_LOGGERCONFIG_HPP */
