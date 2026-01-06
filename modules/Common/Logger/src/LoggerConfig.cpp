/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#include <exception>
#include <expected>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Logger/LoggerConfig.hpp"

namespace medlog
{

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
	 *
	 * @param value: value to read
	 * @param param: output param filled with input value
	 * @throws std::invalid_argument if invalid value for a boolean
	 */
	static constexpr void operator()(std::string_view value, bool& param)
	{
		using namespace std::string_view_literals;

		static const std::unordered_set<std::string_view> trueValues = {"true"sv,
		                                                                "TRUE"sv, "1"sv};
		static const std::unordered_set<std::string_view> falseValues = {
		    "false"sv, "FALSE"sv, "0"sv};

		if (trueValues.find(value) != trueValues.end())
		{
			param = true;
		}
		else if (falseValues.find(value) != falseValues.end())
		{
			param = false;
		}
		else
		{
			throw std::invalid_argument("Invalid value for boolean: " +
			                            std::string(value));
		}
	}

	/**
	 * @brief: Converter to handle LogLevel parameters
	 *
	 * @param value: value to read
	 * @param param: output param filled with input value
	 * @throws std::invalid_argument if invalid value for a boolean
	 */
	static constexpr void operator()(std::string_view value, LogLevel& param)
	{
		auto result = from_string(value, param);
		if (!result)
		{
			throw std::invalid_argument("Invalid value for LogLevel: " +
			                            std::string(value));
		}
	}

	/**
	 * @brief: Converter to handle std::string and std::filesystem::path parameters
	 *
	 * @tparam T The target type, restricted to `std::string` or `std::filesystem::path`.
	 * @param value: value to read
	 * @param param: output param filled with input value
	 */
	template <typename T,
	          typename = std::enable_if_t<
	              std::disjunction_v<std::is_same<T, std::string>,
	                                 std::is_same<T, std::filesystem::path>>>>
	static constexpr void operator()(std::string_view value, T& param)
	{
		param = value;
	}

	/**
	 * @brief: Converter to handle size_t parameters
	 *
	 * @param value: value to read
	 * @param param: output param filled with input value
	 */

	static constexpr void operator()(std::string_view value, std::size_t& param)
	{
		int valueInt;

		auto result =
		    std::from_chars(value.data(), value.data() + value.size(), valueInt);
		if (result.ec == std::errc::invalid_argument)
		{
			throw std::invalid_argument("Invalid value for size_t: " +
			                            std::string(value));
		}
		param = static_cast<std::size_t>(valueInt);
	}

	/**
	 * @brief: Converter to handle chrono::milliseconds parameters
	 *
	 * @param value: value to read
	 * @param param: output param filled with input value
	 */
	static constexpr void operator()(std::string_view value,
	                                 std::chrono::milliseconds& param)
	{
		int valueInt;
		auto result =
		    std::from_chars(value.data(), value.data() + value.size(), valueInt);
		if (result.ec == std::errc::invalid_argument)
		{
			throw std::invalid_argument("Invalid value for milliseconds: " +
			                            std::string(value));
		}
		param = static_cast<std::chrono::milliseconds>(valueInt);
	}
};

// --------------------------------------------------------------------
[[nodiscard]] std::expected<LoggerConfig, std::string> loadConfigurationFile(
    const std::filesystem::path& filename)
{
	using namespace std::string_literals;

	LoggerConfig cfg;

	std::ifstream file(filename);

	Converter converter{};
	std::unordered_map<std::string, std::function<void(const std::string&)>> setters = {
	    {"app_name"s, [&](const std::string& val) { converter(val, cfg.app_name); }},
	    {"log_dir"s, [&](const std::string& val) { converter(val, cfg.log_dir); }},
	    {"log_filename"s,
	     [&](const std::string& val) { converter(val, cfg.log_filename); }},
	    {"error_log_filename"s,
	     [&](const std::string& val) { converter(val, cfg.error_log_filename); }},
	    {"user_event_name"s,
	     [&](const std::string& val) { converter(val, cfg.user_event_name); }},
	    {"user_event_log_filename"s,
	     [&](const std::string& val) { converter(val, cfg.user_event_log_filename); }},
	    {"max_file_size_mebibytes"s,
	     [&](const std::string& val) { converter(val, cfg.max_file_size_mebibytes); }},
	    {"max_files"s, [&](const std::string& val) { converter(val, cfg.max_files); }},
	    {"async_queue_size"s,
	     [&](const std::string& val) { converter(val, cfg.async_queue_size); }},
	    {"thread_count"s,
	     [&](const std::string& val) { converter(val, cfg.thread_count); }},
	    {"level"s, [&](const std::string& val) { converter(val, cfg.level); }},
	    {"flush_every"s,
	     [&](const std::string& val) { converter(val, cfg.flush_every); }},
	    {"pattern"s, [&](const std::string& val) { converter(val, cfg.pattern); }},
	    {"enable_separate_error_log"s,
	     [&](const std::string& val) { converter(val, cfg.enable_separate_error_log); }},
	    {"enable_user_event_log"s,
	     [&](const std::string& val) { converter(val, cfg.enable_user_event_log); }}};

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

				auto found = setters.find(key);
				if (found != setters.end())
				{
					try
					{
						found->second(value);
					}
					catch (...)
					{
						return std::unexpected("loadConfigurationFile: Invalid value " +
						                       value + " for key " + key);
					}
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
