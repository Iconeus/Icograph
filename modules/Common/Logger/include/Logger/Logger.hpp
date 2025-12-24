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

// Initialize once at startup
void initLogger(const LoggerConfig& cfg);

// Obtain the default logger
void log(LogLevel level, std::string_view msg);

template <class... Args>
void trace(std::string_view msg);

void debug(std::string_view msg);
void info(std::string_view msg);
void warn(std::string_view msg);
void error(std::string_view msg);
void critical(std::string_view msg);

// Get logger dedicated to user events
void userEventLog(std::string_view msg, LogLevel level = LogLevel::Info);

// Flush and shutdown spdlog
void shutdown();

}  // namespace medlog

#endif /* LOGGER_LOGGER_HPP */

// TODO : SUPPORT OF MESSAGE WITH PARAMETERS!!!
