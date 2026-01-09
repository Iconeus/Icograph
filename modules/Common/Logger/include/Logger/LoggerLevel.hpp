/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef LOGGER_LOGGERLEVEL_HPP
#define LOGGER_LOGGERLEVEL_HPP

#include <cstdint>
#include <exception>
#include <utility>

namespace medlog
{

/**
 * @enum LogLevel
 * @brief Represents the different log levels supported by the logger.
 */
enum class LogLevel : uint8_t
{
	Trace = 0,
	Debug = 1,
	Info = 2,
	Warn = 3,
	Error = 4,
	Critical = 5,
	Off = 6
};

/**
 * @brief Converts a LogLevel enum value to its string representation.
 * @param type The LogLevel enum value to convert.
 * @return std::string The string representation of the enum value.
 */
constexpr std::string to_string(LogLevel level)
{
	using namespace std::string_literals;

	switch (level)
	{
	case LogLevel::Trace:
		return "Trace"s;
	case LogLevel::Debug:
		return "Debug"s;
	case LogLevel::Info:
		return "Info"s;
	case LogLevel::Warn:
		return "Warn"s;
	case LogLevel::Error:
		return "Error"s;
	case LogLevel::Critical:
		return "Critical"s;
	case LogLevel::Off:
		return "Off"s;
	}

	throw std::domain_error("Invalid value for LogLevel: " + std::to_underlying(level));
}

/**
 * @brief Attempts to convert a string to a LogLevel enum value.
 * @param str The string to convert.
 * @param type Reference to the LogLevel enum value to populate.
 * @return bool True if the conversion was successful, false otherwise.
 */
[[nodiscard]] constexpr bool from_string(std::string_view str, LogLevel& type)
{
	using namespace std::string_view_literals;

	bool status{true};

	if (str == "Trace"sv)
	{
		type = LogLevel::Trace;
	}
	else if (str == "Debug"sv)
	{
		type = LogLevel::Debug;
	}
	else if (str == "Info"sv)
	{
		type = LogLevel::Info;
	}
	else if (str == "Warn"sv)
	{
		type = LogLevel::Warn;
	}
	else if (str == "Error"sv)
	{
		type = LogLevel::Error;
	}
	else if (str == "Critical"sv)
	{
		type = LogLevel::Critical;
	}
	else
	{
		status = false;
	}
	return status;
}

}  // namespace medlog

#endif /* LOGGER_LOGGERLEVEL_HPP */
