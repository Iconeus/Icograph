/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef WORKFLOWTYPE_H
#define WORKFLOWTYPE_H

#include <cstdint>

namespace workflow
{

/**
 * @enum WorkflowType
 * @brief Represents the types of workflows supported in the system.
 */
enum class WorkflowType : uint8_t
{
	NEONATE,
	NEURORADIOLOGY,
	NEUROSURGERY
};

/**
 * @brief Converts a WorkflowType enum value to its string representation.
 * @param type The WorkflowType enum value to convert.
 * @return std::string The string representation of the enum value.
 */
inline std::string to_string(WorkflowType type)
{
	switch (type)
	{
	case WorkflowType::NEONATE:
		return "Neonate";
	case WorkflowType::NEURORADIOLOGY:
		return "Neuroradiology";
	case WorkflowType::NEUROSURGERY:
		return "Neurosurgery";
	default:
		return "Unknown";
	}
}

/**
 * @brief Attempts to convert a string to a WorkflowType enum value.
 * @param str The string to convert.
 * @param type Reference to the WorkflowType enum value to populate.
 * @return bool True if the conversion was successful, false otherwise.
 */
inline bool from_string(std::string_view str, WorkflowType& type)
{
	if (str == "Neonate")
	{
		type = WorkflowType::NEONATE;
		return true;
	}
	else if (str == "Neuroradiology")
	{
		type = WorkflowType::NEURORADIOLOGY;
		return true;
	}
	else if (str == "Neurosurgery")
	{
		type = WorkflowType::NEUROSURGERY;
		return true;
	}
	return false;
}

/**
 * @brief Attempts to convert an integer to a WorkflowType enum value.
 * @param value The integer value to convert.
 * @param type Reference to the WorkflowType enum value to populate.
 * @return bool True if the integer matches a valid enum value, false otherwise.
 */
inline bool from_integer(std::underlying_type_t<WorkflowType> value, WorkflowType& type)
{
	switch (value)
	{
	case static_cast<std::underlying_type_t<WorkflowType>>(WorkflowType::NEONATE):
		type = WorkflowType::NEONATE;
		return true;
	case static_cast<std::underlying_type_t<WorkflowType>>(WorkflowType::NEURORADIOLOGY):
		type = WorkflowType::NEURORADIOLOGY;
		return true;
	case static_cast<std::underlying_type_t<WorkflowType>>(WorkflowType::NEUROSURGERY):
		type = WorkflowType::NEUROSURGERY;
		return true;
	default:
		return false;
	}
}

}  // namespace workflow

#endif  // WORKFLOWTYPE_H
