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
	std::string levelName{""};

	switch (level)
	{
	case LogLevel::Trace:
		levelName = "Trace";
		break;
	case LogLevel::Debug:
		levelName = "Debug";
		break;
	case LogLevel::Info:
		levelName = "Info";
		break;
	case LogLevel::Warn:
		levelName = "Warn";
		break;
	case LogLevel::Error:
		levelName = "Error";
		break;
	case LogLevel::Critical:
		levelName = "Critical";
		break;
	case LogLevel::Off:
		levelName = "Off";
		break;
	}
	return levelName;
}

/**
 * @brief Attempts to convert a string to a WorkflowType enum value.
 * @param str The string to convert.
 * @param type Reference to the WorkflowType enum value to populate.
 * @return bool True if the conversion was successful, false otherwise.
 */
[[nodiscard]] constexpr bool from_string(std::string_view str, LogLevel& type)
{
	bool status{false};
	if (str == "Trace")
	{
		type = LogLevel::Trace;
		status = true;
	}
	else if (str == "Debug")
	{
		type = LogLevel::Debug;
		status = true;
	}
	else if (str == "Info")
	{
		type = LogLevel::Info;
		status = true;
	}
	else if (str == "Warn")
	{
		type = LogLevel::Warn;
		status = true;
	}
	else if (str == "Error")
	{
		type = LogLevel::Error;
		status = true;
	}
	else if (str == "Critical")
	{
		type = LogLevel::Critical;
		status = true;
	}
	return status;
}

}  // namespace medlog

#endif /* LOGGER_LOGGERLEVEL_HPP */
