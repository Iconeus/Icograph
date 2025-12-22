/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef ICOGRAPH_CUSTOMMESSAGEIDENTIFIER_HPP
#define ICOGRAPH_CUSTOMMESSAGEIDENTIFIER_HPP

#include <caf/type_id.hpp>

namespace common_caf
{

/*
 * Centralized Unique Identifiers for Custom CAF Message Types
 *
 * This file defines unique identifiers used to declare custom message types in CAF.
 * By consolidating all identifiers here, we ensure:
 *   - **Uniqueness**: Each identifier is defined in one place, preventing duplicates or
 * conflicts across modules.
 *   - **Maintainability**: Updates or changes to identifiers only need to be made here,
 * reducing the risk of inconsistencies.
 *   - **Clarity**: Developers can easily locate and reference all custom message type
 * IDs, improving code readability and collaboration.
 *
 * Usage:
 *   - When declaring a new custom message type in any module, use an ID from this file.
 *   - Avoid hardcoding IDs in multiple locations to prevent collisions and simplify
 * refactoring.
 *
 * Example:
 *   // In your module's message declaration:
 *   CAF_ADD_TYPE_ID(MyModuleName, MyCustomMessageType,
 * custom_message_type_id::my_custom_message)
 */

inline constexpr auto custom_types_general_id = caf::first_custom_type_id;
inline constexpr auto custom_types_workflow_id = caf::first_custom_type_id + 10;
inline constexpr auto custom_types_acq_module_id = caf::first_custom_type_id + 20;

}  // namespace common_caf

#endif  // ICOGRAPH_CUSTOMMESSAGEIDENTIFIER_HPP
