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
	std::string workflowName{""};

	switch (type)
	{
	case WorkflowType::Neonate:
		workflowName = "Neonate";
		break;
	case WorkflowType::NeuroRadiology:
		workflowName = "NeuroRadiology";
		break;
	case WorkflowType::NeuroSurgery:
		workflowName = "NeuroSurgery";
		break;
	}
	return workflowName;
}

/**
 * @brief Attempts to convert a string to a WorkflowType enum value.
 * @param str The string to convert.
 * @param type Reference to the WorkflowType enum value to populate.
 * @return bool True if the conversion was successful, false otherwise.
 */
[[nodiscard]] constexpr bool from_string(std::string_view str, WorkflowType& type)
{
	bool status{false};
	if (str == "Neonate")
	{
		type = WorkflowType::Neonate;
		status = true;
	}
	else if (str == "NeuroRadiology")
	{
		type = WorkflowType::NeuroRadiology;
		status = true;
	}
	else if (str == "NeuroSurgery")
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

#endif  // WORKFLOWMANAGER_WORKFLOWTYPE_HPP
