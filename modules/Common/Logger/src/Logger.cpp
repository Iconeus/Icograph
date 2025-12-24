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
#include "Logger/LoggerConfig.hpp"

namespace medlog
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

// Loggers
static std::shared_ptr<spdlog::logger> _application_logger = nullptr;
static std::shared_ptr<spdlog::logger> _user_event_logger = nullptr;
static std::once_flag _init_log_once;
static bool _logger_initialized = false;

constexpr spdlog::level::level_enum convertLogLevel(LogLevel level)
{
	return spdlog::level::level_enum(level);
}

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

void initLoggerImpl(const LoggerConfig& cfg)
{
	std::error_code ec;
	std::filesystem::create_directories(cfg.log_dir, ec);
	bool directoryCreated = std::filesystem::create_directories(cfg.log_dir, ec);
	if (!directoryCreated && ec)
	{
		throw std::runtime_error("Cannot create log directory: " + cfg.log_dir +
		                         ". Error returned: " + ec.message());
	}

	// In this implementation, all the loggers will use the same thread pool
	spdlog::init_thread_pool(cfg.async_queue_size, cfg.thread_count);
	spdlog::flush_every(cfg.flush_every);
	spdlog::flush_on(spdlog::level::critical);

	// Creates potentially several targets for the application messages
	std::vector<spdlog::sink_ptr> appSinks;

	// Flush in application logfile
	std::string appLogfilePath = std::format("{}/{}", cfg.log_dir, cfg.log_filename);
	auto appLogFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
	    appLogfilePath, cfg.max_file_size_bytes, cfg.max_files);
	appLogFileSink->set_level(convertLogLevel(cfg.level));
	appSinks.push_back(appLogFileSink);

	// Flush also in console if configured
	if (cfg.enable_console_output)
	{
		auto consoleSink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
		consoleSink->set_level(convertLogLevel(cfg.level));
		appSinks.push_back(consoleSink);
	}

	// Flush in an additional file dedicated for error and critical messages if
	// configured
	if (cfg.enable_separate_error_log)
	{
		std::string errorLogfilePath =
		    std::format("{}/{}", cfg.log_dir, cfg.error_log_filename);

		auto errorLogFileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
		    errorLogfilePath, cfg.max_file_size_bytes, cfg.max_files);
		errorLogFileSink->set_level(spdlog::level::err);
		appSinks.push_back(errorLogFileSink);
	}

	/// Create application logger
	_application_logger = std::make_shared<spdlog::async_logger>(
	    cfg.app_name, appSinks.begin(), appSinks.end(), spdlog::thread_pool(),
	    spdlog::async_overflow_policy::block);
	_application_logger->set_pattern(cfg.pattern);

	spdlog::set_default_logger(_application_logger);  // Also register the logger

	if (cfg.enable_user_event_log)
	{
		/// Add an additional logger for user events for audit trail
		initUserEventLogger(cfg);
	}

	_logger_initialized = true;
}

void initLogger(const LoggerConfig& cfg)
{
	std::call_once(_init_log_once, [&]() { initLoggerImpl(cfg); });
}

inline void checkLogInitialized()
{
	assert(_logger_initialized && "Logger not initialized. Call initLogger() first.");
}

void log(LogLevel level, std::string_view msg)
{
	checkLogInitialized();
	_application_logger->log(convertLogLevel(level), msg);
};

void trace(std::string_view msg)
{
	checkLogInitialized();
	_application_logger->trace(msg);
}

void debug(std::string_view msg)
{
	checkLogInitialized();
	_application_logger->debug(msg);
}

void info(std::string_view msg)
{
	checkLogInitialized();
	_application_logger->info(msg);
}

void warn(std::string_view msg)
{
	checkLogInitialized();
	_application_logger->warn(msg);
}
void error(std::string_view msg)
{
	checkLogInitialized();
	_application_logger->error(msg);
}

void critical(std::string_view msg)
{
	checkLogInitialized();
	_application_logger->critical(msg);
}

void userEventLog(std::string_view msg)
{
	checkLogInitialized();
	assert((_user_event_logger != nullptr) && "User event logger not initialized.");

	_user_event_logger->info(msg);
}

void shutdown()
{
	spdlog::drop_all();
	_application_logger.reset();
	_user_event_logger.reset();
	spdlog::shutdown();
}

}  // namespace medlog
