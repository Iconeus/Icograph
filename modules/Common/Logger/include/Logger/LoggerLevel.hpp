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

}  // namespace medlog

#endif /* LOGGER_LOGGERLEVEL_HPP */
