/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>

#include "Logger/Logger.hpp"

namespace medlog
{

namespace detail
{

// clang-format off
static_assert(int(LogLevel::Trace)    == int(spdlog::level::level_enum::trace));
static_assert(int(LogLevel::Debug)    == int(spdlog::level::level_enum::debug));
static_assert(int(LogLevel::Info)     == int(spdlog::level::level_enum::info));
static_assert(int(LogLevel::Warn)     == int(spdlog::level::level_enum::warn));
static_assert(int(LogLevel::Error)    == int(spdlog::level::level_enum::err));
static_assert(int(LogLevel::Critical) == int(spdlog::level::level_enum::critical));
static_assert(int(LogLevel::Off)      == int(spdlog::level::level_enum::off));
// clang-format on

/// @brief Holds the logger configuration
static LoggerConfig _cfg;

/// @brief Flag for log initialization
static std::atomic<bool> _logger_initialized{false};

// --------------------------------------------------------------------
/**
 * @brief: Convert custom LogLevel type into spdlog level
 * @param level The log level
 * @return converted log level
 */
constexpr spdlog::level::level_enum convertLogLevel(LogLevel level) noexcept
{
	return spdlog::level::level_enum(level);
}

// --------------------------------------------------------------------
[[nodiscard]] bool shouldLog(LogLevel level) noexcept
{
	assert(_logger_initialized && "Logger not initialized. Call initLogger() first.");

	return spdlog::should_log(convertLogLevel(level));
}

// --------------------------------------------------------------------
[[nodiscard]] bool shouldLogUserEvent() noexcept
{
	assert(_logger_initialized && "Logger not initialized. Call initLogger() first.");

	return true;
}

// --------------------------------------------------------------------
void trace(std::string_view msg)
{
	spdlog::trace(msg);
}
// --------------------------------------------------------------------
void debug(std::string_view msg)
{
	spdlog::debug(msg);
}
// --------------------------------------------------------------------
void info(std::string_view msg)
{
	spdlog::info(msg);
}
// --------------------------------------------------------------------
void warn(std::string_view msg)
{
	spdlog::warn(msg);
}
// --------------------------------------------------------------------
void error(std::string_view msg)
{
	spdlog::error(msg);
}
// --------------------------------------------------------------------
void critical(std::string_view msg)
{
	spdlog::critical(msg);
}
// --------------------------------------------------------------------
void userEvent(std::string_view msg)
{
	spdlog::get(_cfg.user_event_name)->info(msg);
}

// --------------------------------------------------------------------
/**
 * @brief: Initialize the dedicated logger for user events, setting the level to info.
 * @param cfg The logging configuration
 */
void initUserEventLogger(const LoggerConfig& cfg)
{
	std::filesystem::path userEventLogfilePath{cfg.log_dir};
	userEventLogfilePath /= cfg.user_event_log_filename;

	auto userEventLogFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
	    userEventLogfilePath, cfg.max_file_size_bytes, cfg.max_files);

	auto user_event_logger = std::make_shared<spdlog::async_logger>(
	    cfg.user_event_name, userEventLogFileSink, spdlog::thread_pool(),
	    spdlog::async_overflow_policy::block);
	user_event_logger->set_pattern(cfg.pattern);
	user_event_logger->set_level(spdlog::level::info);

	spdlog::register_logger(user_event_logger);
}

}  // namespace detail

// --------------------------------------------------------------------
void initLogger(const LoggerConfig& cfg)
{
	assert((detail::_logger_initialized == false) && "Logger is already initialized.");

	// Keep configuration
	detail::_cfg = cfg;

	std::error_code ec;
	std::filesystem::create_directories(cfg.log_dir, ec);
	if (ec)
	{
		throw std::runtime_error("Cannot create log directory: " + cfg.log_dir +
		                         ". Error returned: " + ec.message());
	}

	// In this implementation, all the loggers will use the same thread pool
	spdlog::init_thread_pool(cfg.async_queue_size, cfg.thread_count);
	spdlog::flush_every(cfg.flush_every);
	spdlog::flush_on(spdlog::level::err);

	// Creates potentially several targets for the application messages
	std::vector<spdlog::sink_ptr> appSinks;

	// Flush in application logfile
	std::filesystem::path appLogfilePath{cfg.log_dir};
	appLogfilePath /= cfg.log_filename;

	auto appLogFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
	    appLogfilePath, cfg.max_file_size_bytes, cfg.max_files);

	// All levels should be logged in the file
	appLogFileSink->set_level(spdlog::level::trace);
	appSinks.push_back(appLogFileSink);

	// Flush in an additional file dedicated for error and critical messages if
	// configured
	if (cfg.enable_separate_error_log)
	{
		std::filesystem::path errorLogfilePath{cfg.log_dir};
		errorLogfilePath /= cfg.error_log_filename;

		auto errorLogFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
		    errorLogfilePath, cfg.max_file_size_bytes, cfg.max_files);

		// Only error and critical levels should be logged in this file
		errorLogFileSink->set_level(spdlog::level::err);
		appSinks.push_back(errorLogFileSink);
	}

	/// Create application logger
	auto application_logger = std::make_shared<spdlog::async_logger>(
	    cfg.app_name, appSinks.begin(), appSinks.end(), spdlog::thread_pool(),
	    spdlog::async_overflow_policy::block);
	application_logger->set_pattern(cfg.pattern);
	application_logger->set_level(detail::convertLogLevel(cfg.level));

	// Set default logger. This will also register it.
	spdlog::set_default_logger(application_logger);

	// If enabled, configure the logger for user events
	if (cfg.enable_user_event_log)
	{
		detail::initUserEventLogger(cfg);
	}

	detail::_logger_initialized = true;
}

// --------------------------------------------------------------------
void shutdown()
{
	spdlog::drop_all();
	spdlog::shutdown();
}

}  // namespace medlog
