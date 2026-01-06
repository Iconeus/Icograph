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

using namespace std::string_literals;

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
	std::string app_name = "app"s;
	std::filesystem::path log_dir = L"logs"s;
	std::filesystem::path log_filename = L"app.log"s;
	std::filesystem::path error_log_filename = L"error.log"s;
	std::string user_event_name = "UserEvent"s;
	std::filesystem::path user_event_log_filename = L"UserEvent.log"s;

	// Rotation settings
	std::size_t max_file_size_mebibytes = 50ULL;  // 50 MiB
	std::size_t max_files = 10;

	// Async queue size (tradeoff between memory and drop risk).
	std::size_t async_queue_size = 8192;

	// Number of worker thread. A value of 1 preserves the message order after dequeuing
	std::size_t thread_count = 1;

	// Minimum level written.
	LogLevel level = LogLevel::Info;

	std::chrono::milliseconds flush_every{1000};  // Every second

	// Pattern: Date ISO8601, thread id, level, logger name, message
	std::string pattern = "[%Y-%m-%d %H:%M:%S.%e][%t][%-8l]%v"s;

	// Useful logs for audit trail
	bool enable_separate_error_log = false;
	bool enable_user_event_log = false;
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
