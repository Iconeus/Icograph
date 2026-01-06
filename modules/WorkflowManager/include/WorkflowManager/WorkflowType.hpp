/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef WORKFLOWMANAGER_WORKFLOWTYPE_HPP
#define WORKFLOWMANAGER_WORKFLOWTYPE_HPP

#include <cstdint>
#include <exception>
#include <format>
#include <string>
#include <utility>

namespace workflow
{

/**
 * @enum WorkflowType
 * @brief Represents the types of workflows supported in the system.
 */
enum class WorkflowType : uint8_t
{
	Neonate,
	NeuroRadiology,
	NeuroSurgery
};

/**
 * @brief Converts a WorkflowType enum value to its string representation.
 * @param type The WorkflowType enum value to convert.
 * @return std::string The string representation of the enum value.
 */
constexpr std::string to_string(WorkflowType type)
{
	using namespace std::string_literals;

	switch (type)
	{
	case WorkflowType::Neonate:
		return "Neonate"s;
	case WorkflowType::NeuroRadiology:
		return "NeuroRadiology"s;
	case WorkflowType::NeuroSurgery:
		return "NeuroSurgery"s;
	}

	// throw exception et rajouter return dans chaque case
	throw std::domain_error("Invalid value for WorkflowType: " +
	                        std::to_underlying(type));
}

/**
 * @brief Attempts to convert a string to a WorkflowType enum value.
 * @param str The string to convert.
 * @param type Reference to the WorkflowType enum value to populate.
 * @return bool True if the conversion was successful, false otherwise.
 */
[[nodiscard]] constexpr bool from_string(std::string_view str, WorkflowType& type)
{
	using namespace std::string_view_literals;

	bool status{false};
	if (str == "Neonate"sv)
	{
		type = WorkflowType::Neonate;
		status = true;
	}
	else if (str == "NeuroRadiology"sv)
	{
		type = WorkflowType::NeuroRadiology;
		status = true;
	}
	else if (str == "NeuroSurgery"sv)
	{
		type = WorkflowType::NeuroSurgery;
		status = true;
	}
	return status;
}

/**
 * @brief Attempts to convert an integer to a WorkflowType enum value.
 * @param value The integer value to convert.
 * @param type Reference to the WorkflowType enum value to populate.
 * @return bool True if the integer matches a valid enum value, false otherwise.
 */
[[nodiscard]] constexpr bool from_integer(std::underlying_type_t<WorkflowType> value,
                                          WorkflowType& type)
{
	bool status{false};

	switch (value)
	{
	case std::to_underlying(WorkflowType::Neonate):
		type = WorkflowType::Neonate;
		status = true;
		break;
	case std::to_underlying(WorkflowType::NeuroRadiology):
		type = WorkflowType::NeuroRadiology;
		status = true;
		break;
	case std::to_underlying(WorkflowType::NeuroSurgery):
		type = WorkflowType::NeuroSurgery;
		status = true;
		break;
	}

	return status;
}

}  // namespace workflow

/**
 * @brief Specialization of the std::format for WorkflowType. Needed for logging
 */
template <>
struct std::formatter<workflow::WorkflowType>
{
	constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

	auto format(const workflow::WorkflowType& type, std::format_context& ctx) const
	{
		return std::format_to(ctx.out(), "{}", workflow::to_string(type));
	}
};

#endif  // WORKFLOWMANAGER_WORKFLOWTYPE_HPP
