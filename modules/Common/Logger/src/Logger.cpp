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
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <system_error>
#include <vector>

#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
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

/// @brief Main logger of the application
static std::shared_ptr<spdlog::logger> _application_logger = nullptr;

/// @brief Logger dedicated to user events for audit trail
static std::shared_ptr<spdlog::logger> _user_event_logger = nullptr;

/// @brief Flag necessary for std::call_once to ensure initializaton is only performed
/// once
static std::once_flag _init_log_once;

/// @brief Flag for log initialization
static bool _logger_initialized = false;

// --------------------------------------------------------------------
/**
 * @brief: Convert custom LogLevel type into spdlog level
 * @param level The log level
 * @return converted log level
 */
constexpr spdlog::level::level_enum convertLogLevel(LogLevel level)
{
	return spdlog::level::level_enum(level);
}

// --------------------------------------------------------------------
/**
 * @brief: Initialize the dedicated logger for user events, setting the level to info.
 * @param cfg The logging configuration
 */
void initUserEventLogger(const LoggerConfig& cfg)
{
	std::string userEventLogfilePath =
	    std::format("{}/{}", cfg.log_dir, cfg.user_event_log_filename);

	auto userEventLogFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
	    userEventLogfilePath, cfg.max_file_size_bytes, cfg.max_files);

	_user_event_logger = std::make_shared<spdlog::async_logger>(
	    cfg.user_event_name, userEventLogFileSink, spdlog::thread_pool(),
	    spdlog::async_overflow_policy::block);
	_user_event_logger->set_pattern(cfg.pattern);
	_user_event_logger->set_level(spdlog::level::info);

	spdlog::register_logger(_user_event_logger);
}

// --------------------------------------------------------------------
bool shouldLog(LogLevel level)
{
	assert(_logger_initialized && _application_logger &&
	       "Logger not initialized. Call initLogger() first.");

	return _application_logger->should_log(convertLogLevel(level));
}

// --------------------------------------------------------------------
bool shouldLogUserEvent()
{
	assert((_user_event_logger != nullptr) && "User event logger not initialized.");

	return _user_event_logger != nullptr;
}

// --------------------------------------------------------------------
void trace(std::string_view msg)
{
	_application_logger->trace(msg);
}
// --------------------------------------------------------------------
void debug(std::string_view msg)
{
	_application_logger->debug(msg);
}
// --------------------------------------------------------------------
void info(std::string_view msg)
{
	_application_logger->info(msg);
}
// --------------------------------------------------------------------
void warn(std::string_view msg)
{
	_application_logger->warn(msg);
}
// --------------------------------------------------------------------
void error(std::string_view msg)
{
	_application_logger->error(msg);
}
// --------------------------------------------------------------------
void critical(std::string_view msg)
{
	_application_logger->critical(msg);
}
// --------------------------------------------------------------------
void userEvent(std::string_view msg)
{
	assert((_user_event_logger != nullptr) && "User event logger not initialized.");
	_user_event_logger->info(msg);
}

}  // namespace detail

// --------------------------------------------------------------------
void initLogger(const LoggerConfig& cfg)
{
	std::call_once(
	    detail::_init_log_once,
	    [&]()
	    {
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
		    std::string appLogfilePath =
		        std::format("{}/{}", cfg.log_dir, cfg.log_filename);
		    auto appLogFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
		        appLogfilePath, cfg.max_file_size_bytes, cfg.max_files);
		    // All levels should be logged in the file
		    appLogFileSink->set_level(spdlog::level::trace);
		    appSinks.push_back(appLogFileSink);

		    // Flush in an additional file dedicated for error and critical messages if
		    // configured
		    if (cfg.enable_separate_error_log)
		    {
			    std::string errorLogfilePath =
			        std::format("{}/{}", cfg.log_dir, cfg.error_log_filename);

			    auto errorLogFileSink =
			        std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
			            errorLogfilePath, cfg.max_file_size_bytes, cfg.max_files);
			    // Only error and critical levels should be logged in this file
			    errorLogFileSink->set_level(spdlog::level::err);
			    appSinks.push_back(errorLogFileSink);
		    }

		    /// Create application logger
		    detail::_application_logger = std::make_shared<spdlog::async_logger>(
		        cfg.app_name, appSinks.begin(), appSinks.end(), spdlog::thread_pool(),
		        spdlog::async_overflow_policy::block);
		    detail::_application_logger->set_pattern(cfg.pattern);
		    detail::_application_logger->set_level(detail::convertLogLevel(cfg.level));

		    // Set default logger. This will also register it.
		    spdlog::set_default_logger(detail::_application_logger);

		    // If enabled, configure the logger for user events
		    if (cfg.enable_user_event_log)
		    {
			    detail::initUserEventLogger(cfg);
		    }

		    detail::_logger_initialized = true;
	    });
}

// --------------------------------------------------------------------
void shutdown()
{
	spdlog::drop_all();
	detail::_application_logger.reset();
	detail::_user_event_logger.reset();
	spdlog::shutdown();
}

}  // namespace medlog
