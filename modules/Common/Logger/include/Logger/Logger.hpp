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

#include "LoggerConfig.hpp"

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
 */
bool shouldLog(LogLevel level);

// Logging functions implementation per level
void traceStr(std::string_view msg);
void debugStr(std::string_view msg);
void infoStr(std::string_view msg);
void warnStr(std::string_view msg);
void errorStr(std::string_view msg);
void criticalStr(std::string_view msg);
void userEventStr(std::string_view msg);

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
 * @param fmt The format string
 * @param args The arguments to format into the message.
 *
 * @note
 * This function avoids expensive formatting operations if the log level is disabled.
 */
template <LogLevel Level, typename Func, typename... Args>
void logIfEnabled(Func func, std::string_view fmt, Args&&... args)
{
	if (shouldLog(Level))
	{
		func(std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));
	}
}

}  // namespace detail

/**
 * @brief: Initialize the logger for the current process.
 * \warning This function should always be called in the application before logging.
 *          Uses std::call_once to ensure that a single initialization is performed even
 *          if several threads call it.
 * @param cfg: configuration of the logger
 */
void initLogger(const LoggerConfig& cfg);

/**
 * @brief: Flush remaining logs in the buffer and shutdown spdlog
 * \warning This function should always be called before closing the application.
 */
void shutdown();

/**
 * @brief: User availables logging function per log level
 * @param fmt The format string
 * @param args The arguments to format into the message.
 */

template <typename... Args>
void trace(std::string_view fmt, Args&&... args)
{
	detail::logIfEnabled<LogLevel::Trace>(detail::traceStr, fmt,
	                                      std::forward<Args>(args)...);
}

template <typename... Args>
void debug(std::string_view fmt, Args&&... args)
{
	detail::logIfEnabled<LogLevel::Debug>(detail::debugStr, fmt,
	                                      std::forward<Args>(args)...);
}

template <typename... Args>
void info(std::string_view fmt, Args&&... args)
{
	detail::logIfEnabled<LogLevel::Info>(detail::infoStr, fmt,
	                                     std::forward<Args>(args)...);
}

template <typename... Args>
void warn(std::string_view fmt, Args&&... args)
{
	detail::logIfEnabled<LogLevel::Warn>(detail::warnStr, fmt,
	                                     std::forward<Args>(args)...);
}

template <typename... Args>
void error(std::string_view fmt, Args&&... args)
{
	detail::logIfEnabled<LogLevel::Error>(detail::errorStr, fmt,
	                                      std::forward<Args>(args)...);
}

template <typename... Args>
void critical(std::string_view fmt, Args&&... args)
{
	detail::logIfEnabled<LogLevel::Critical>(detail::criticalStr, fmt,
	                                         std::forward<Args>(args)...);
}

/**
 * @brief: Logger specialization for user events. Call directly userEventStr as checks
 * should be performed on the dedicated logger for user events.
 * @param fmt The format string
 * @param args The arguments to format into the message.
 */
template <typename... Args>
void userEvent(std::string_view fmt, Args&&... args)
{
	userEventStr(std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));
}

}  // namespace medlog

#endif /* LOGGER_LOGGER_HPP */
