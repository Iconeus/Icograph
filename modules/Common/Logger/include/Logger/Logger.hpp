/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef LOGGER_LOGGER_HPP
#define LOGGER_LOGGER_HPP

#include <source_location>

#include "LoggerConfig.hpp"
#include "LoggerLevel.hpp"

/**
 * @brief Logging library.
 * Leverages spdlog registry Singleton to keep an instance of the registered loggers per
 * process.
 *
 * Usage:
 *   1. Create configuration: "medlog::LoggerConfig cfg".
 *   2. Modify its parameters if necessary.
 *   3. Create a logger object "medlog::Logger logger(cfg)"
 *   4. Use provided MEDLOG_* macros from anywhere to push logging request in spdlog queue
 *   5. Destruction of the logger when out of scope will perform the cleanup (global
 *      static variables, spdlog registry, sinks etc)
 */

namespace medlog
{

// Implementation details.
// Should not be called directly from outside.
namespace detail
{
/**
 * @brief Helper to check if logger has been properly initialized and if the specified log
 * level is enabled.
 * @param level log level to check.
 * @return false if log is under the level to be logged
 *
 * @throws std::logic_error if the logger is not initialized
 */
[[nodiscard]] bool shouldLog(LogLevel level);

/**
 * @brief Helper to check if logger for user events has been properly initialized
 *
 * @throws std::logic_error if the logger is not initialized
 */
[[nodiscard]] bool shouldLogUserEvent();

// Logging functions implementation per level. Use the default logger setup in the
// initLogger function
void trace(std::string_view msg);
void debug(std::string_view msg);
void info(std::string_view msg);
void warn(std::string_view msg);
void error(std::string_view msg);
void critical(std::string_view msg);
void userEvent(std::string_view msg);

/**
 * @brief Internal helper function to log a message if the specified log level is enabled.
 *
 * This function checks if the given log level is enabled (via `detail::shouldLog`).
 * If enabled, it formats the message using `std::vformat` and forwards it to the
 * appropriate logging function.
 *
 * @tparam Level The log level to check (e.g., `LogLevel::Info`, `LogLevel::Debug`).
 * @tparam Func Type of the logging function (e.g., `detail::infoStr`).
 * @tparam Args Types of the format arguments.
 *
 * @param func The logging function to call if the level is enabled (e.g.,
 * `detail::infoStr`).
 * @param loc The source location of the log message.
 * @param fmt The format string
 * @param args The arguments to format into the message.
 *
 * @throws std::logic_error if the logger is not initialized.
 *
 * @note
 * - This function avoids expensive formatting operations if the log level is disabled.
 * - The injection of the filename/line is done here because spdlog does not provide a
 *   nice way to add those information in the log without having to use SPDLOG_* macros
 *   and thus exposing spdlog to the full application.
 */
template <LogLevel Level, typename Func, typename... Args>
void logIfEnabled(Func func,
                  const std::source_location& loc,
                  std::string_view fmt,
                  Args&&... args)
{
	if (shouldLog(Level))
	{
		const std::string message = std::vformat(fmt, std::make_format_args(args...));

		// Truncate the file name to the last characters
		constexpr std::size_t filenameSize{30};
		std::string fileName = loc.file_name();
		if (fileName.length() > filenameSize)
		{
			fileName = fileName.substr(fileName.length() - filenameSize);
		}
		func(std::format("[{}:{}] {}", fileName, loc.line(), message));
	}
}

/**
 * @brief Internal helper specialized function for user event logs.
 *
 * @tparam Args Types of the format arguments.
 * @param fmt The format string
 * @param args The arguments to format into the message.
 *
 * @note
 * - Those logs shall always be enabled so there is no check of the log level. Only
 *   proper initialization of the logger is verified.
 * - File location is not necessary for this type of logs
 */
template <typename... Args>
void logUserEvent(std::string_view fmt, Args&&... args)
{
	if (shouldLogUserEvent())
	{
		userEvent(std::vformat(fmt, std::make_format_args(args...)));
	}
}

}  // namespace detail

// --------------------------------------------------------------------
//
// C L A S S   L O G G E R
//
// --------------------------------------------------------------------

/**
 * \class Logger
 *
 * @brief Class for logging resources lifetime management. The creation of the logger
 * instantiates the spdlog environment (sinks, logger, configuration). The destructor of
 * this object performs the necessary cleanup actions to prevent memory leaks (RAII
 * idiom).
 */
class Logger
{
public:
	/**
	 * @brief: Ctor that initialize the logger for the current process.
	 * \throws exception upon failure (RAII principle)
	 */
	explicit Logger(const LoggerConfig& cfg);
	// Dtor
	~Logger();

	// Do not allow other types of ctor/assignment operators
	Logger() = delete;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator=(Logger&&) = delete;

private:
	/**
	 * @brief: Initialize the logger for the current process.
	 * \warning This function should always be called in the application before logging.
	 *          Uses std::call_once to ensure that a single initialization is performed
	 * even if several threads call it.
	 * @param cfg: configuration of the logger
	 */
	void initLogger(const LoggerConfig& cfg);

	/**
	 * @brief: Initialize the logger dedicated to user events for the current process.
	 * @param cfg: configuration of the logger
	 */
	void initUserEventLogger(const LoggerConfig& cfg);

	/**
	 * @brief: Flush remaining logs in the buffer and shutdown spdlog
	 * \warning This function should always be called before closing the application.
	 */
	void shutdown();
};

}  // namespace medlog

// --------------------------------------------------------------------
//
// M A C R O S
//
// --------------------------------------------------------------------
/**
 * @brief: User availables macros per log level. Macros were the only option to be able to
 * retrieve the current location of the log combined with variadic parameters.
 *
 * @param fmt The format string
 * @param args The arguments to format into the message.
 *
 * @note: __VA_OPT__(, ) __VA_ARGS__ is the C++20 compatible way to handle the case of
 * empty variadic parameters (for instance a call to MEDLOG_TRACE("My message");)
 */
#define MEDLOG_TRACE(fmt, ...)                                    \
	medlog::detail::logIfEnabled<::medlog::LogLevel::Trace>(      \
	    ::medlog::detail::trace, std::source_location::current(), \
	    fmt __VA_OPT__(, ) __VA_ARGS__)

#define MEDLOG_DEBUG(fmt, ...)                                    \
	medlog::detail::logIfEnabled<::medlog::LogLevel::Debug>(      \
	    ::medlog::detail::debug, std::source_location::current(), \
	    fmt __VA_OPT__(, ) __VA_ARGS__)

#define MEDLOG_INFO(fmt, ...)                                    \
	medlog::detail::logIfEnabled<::medlog::LogLevel::Info>(      \
	    ::medlog::detail::info, std::source_location::current(), \
	    fmt __VA_OPT__(, ) __VA_ARGS__)

#define MEDLOG_WARN(fmt, ...)                                    \
	medlog::detail::logIfEnabled<::medlog::LogLevel::Warn>(      \
	    ::medlog::detail::warn, std::source_location::current(), \
	    fmt __VA_OPT__(, ) __VA_ARGS__)

#define MEDLOG_ERROR(fmt, ...)                                    \
	medlog::detail::logIfEnabled<::medlog::LogLevel::Error>(      \
	    ::medlog::detail::error, std::source_location::current(), \
	    fmt __VA_OPT__(, ) __VA_ARGS__)

#define MEDLOG_CRITICAL(fmt, ...)                                    \
	medlog::detail::logIfEnabled<::medlog::LogLevel::Critical>(      \
	    ::medlog::detail::critical, std::source_location::current(), \
	    fmt __VA_OPT__(, ) __VA_ARGS__)

#define MEDLOG_USER_EVENT(fmt, ...) \
	medlog::detail::logUserEvent(fmt __VA_OPT__(, ) __VA_ARGS__)

#endif /* LOGGER_LOGGER_HPP */
