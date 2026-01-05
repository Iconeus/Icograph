/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#include <expected>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "Logger/LoggerConfig.hpp"

namespace medlog
{

enum class parse_error
{
	invalid_input,
	overflow
};

// Traits for milliseconds check
template <typename T>
struct is_milliseconds : std::false_type
{
};

template <>
struct is_milliseconds<std::chrono::milliseconds> : std::true_type
{
};

/**
 * \struct Converter
 *
 * @brief Set of conversion operators used to convert string parameters into their
 * respective types.
 */
struct Converter
{
	/**
	 * @brief: Converter to handle boolean parameters
	 */
	void operator()(std::string_view value, bool& param) const
	{
		if (value == "true")
		{
			param = true;
		}
		else if (value == "false")
		{
			param = false;
		}
	}

	/**
	 * @brief: Converter to handle filesystem::path parameters
	 */
	void operator()(std::string_view value, std::filesystem::path& param) const
	{
		param = value;
	}

	/**
	 * @brief: Converter to handle std::string parameters
	 */
	void operator()(std::string_view value, std::string& param) const { param = value; }

	/**
	 * @brief: Converter to handle LogLevel parameters
	 */
	void operator()(std::string_view value, LogLevel& param) const
	{
		auto result = from_string(value, param);
		std::cout << "TODO " << result << std::endl;
	}

	/**
	 * @brief: Converter to handle size_t and chrono::milliseconds parameters
	 */
	template <typename T,
	          typename = std::enable_if_t<
	              std::disjunction_v<std::is_same<T, size_t>, is_milliseconds<T>>>>
	void operator()(std::string_view value, T& param) const
	{
		int valueInt;
		auto result =
		    std::from_chars(value.data(), value.data() + value.size(), valueInt);
		if (result.ec == std::errc::invalid_argument)
		{
			std::cout << "Could not convert.";
		}
		param = static_cast<T>(valueInt);
	}
};

// --------------------------------------------------------------------
[[nodiscard]] std::expected<LoggerConfig, std::string> loadConfigurationFile(
    const std::filesystem::path& filename)
{
	LoggerConfig cfg;

	std::ifstream file(filename);

	Converter converter{};
	std::unordered_map<std::string, std::function<void(const std::string&)>> setters = {
	    {"app_name", [&](const std::string& val) { converter(val, cfg.app_name); }},
	    {"log_dir", [&](const std::string& val) { converter(val, cfg.log_dir); }},
	    {"log_filename",
	     [&](const std::string& val) { converter(val, cfg.log_filename); }},
	    {"error_log_filename",
	     [&](const std::string& val) { converter(val, cfg.error_log_filename); }},
	    {"user_event_name",
	     [&](const std::string& val) { converter(val, cfg.user_event_name); }},
	    {"user_event_log_filename",
	     [&](const std::string& val) { converter(val, cfg.user_event_log_filename); }},
	    {"max_file_size_megabytes",
	     [&](const std::string& val) { converter(val, cfg.max_file_size_megabytes); }},
	    {"max_files", [&](const std::string& val) { converter(val, cfg.max_files); }},
	    {"async_queue_size",
	     [&](const std::string& val) { converter(val, cfg.async_queue_size); }},
	    {"thread_count",
	     [&](const std::string& val) { converter(val, cfg.thread_count); }},
	    {"level", [&](const std::string& val) { converter(val, cfg.level); }},
	    {"flush_every", [&](const std::string& val) { converter(val, cfg.flush_every); }},
	    {"pattern", [&](const std::string& val) { converter(val, cfg.pattern); }},
	    {"enable_separate_error_log",
	     [&](const std::string& val) { converter(val, cfg.enable_separate_error_log); }},
	    {"enable_user_event_log",
	     [&](const std::string& val) { converter(val, cfg.enable_user_event_log); }},
	    {"flush_every", [&](const std::string& val) { converter(val, cfg.flush_every); }},
	};

	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			if (line.empty() || line[0] == '#')
				continue;  // Skip comments/empty lines
			std::istringstream isLine(line);
			std::string key, value;
			if (std::getline(isLine, key, '=') && std::getline(isLine, value))
			{
				// Trim whitespace
				key.erase(0, key.find_first_not_of(" \t"));
				key.erase(key.find_last_not_of(" \t") + 1);
				value.erase(0, value.find_first_not_of(" \t"));
				value.erase(value.find_last_not_of(" \t") + 1);

				std::cout << " KEY-" << key << "---" << std::endl;
				std::cout << " VALUE-" << value << "---" << std::endl;

				if (setters.find(key) != setters.end())
				{
					setters[key](value);
				}
				else
				{
					return std::unexpected("loadConfigurationFile: Invalid key " + key);
				}
			}
		}
	}

	return cfg;
}
}  // namespace medlog
