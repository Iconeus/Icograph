/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef CAF_CUSTOMACTORIDENTIFIER_HPP
#define CAF_CUSTOMACTORIDENTIFIER_HPP

#include <caf/fwd.hpp>

namespace common_caf
{

/**
 * @file customactoridentifier.hpp
 * @brief Defines custom actor IDs for use in the CAF actor registry.
 *
 * This header provides unique, predefined actor IDs for specific actor types in the
 * application. These IDs are used to register and lookup actors in the CAF actor
 * registry, ensuring consistent and efficient actor identification across the system.
 *
 * @note
 * - Each ID is defined as a `constexpr caf::actor_id` for compile-time safety and
 * performance.
 * - These IDs should be used when spawning or looking up actors in the registry
 *   to avoid dynamic ID assignment and ensure predictable actor addressing.
 * - The values are chosen to avoid conflicts with CAF's built-in IDs and other custom
 * IDs.
 *
 * @warning
 * Do not reuse or reassign these IDs for other actor types.
 * Changing these values after deployment may break actor communication.
 */

/** @brief Unique ID for the workflow manager actor. */
constexpr auto custom_workflow_manager_actor_id = caf::actor_id(10);

/** @brief Unique ID for the echo viewer actor. */
constexpr auto custom_echo_viewer_actor_id = caf::actor_id(20);

/** @brief Unique ID for the domain model actor. */
constexpr auto custom_domain_model_actor_id = caf::actor_id(30);

}  // namespace common_caf

#endif  // CAF_CUSTOMACTORIDENTIFIER_HPP
