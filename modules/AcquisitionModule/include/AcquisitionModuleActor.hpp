/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef ACQUISITIONMODULEACTOR_HPP
#define ACQUISITIONMODULEACTOR_HPP

#include <caf/typed_event_based_actor.hpp>

#include "AcquisitionModuleTypeIds.hpp"

namespace acq_module
{

// Definition of the messaging interface of the acquisition module necessary to
// create the statically typed actor.
// /!\ CAF requires the argument to be written without the cv qualifiers.
struct acq_module_trait
{
	using signatures =
	    caf::type_list<caf::result<void>(acq_request, int32_t, std::vector<caf::actor>)>;
};

// Definition of the statically typed actor
using acq_module_actor = caf::typed_actor<acq_module_trait>;

/**
 * @brief: Defines the callbacks upon message reception. In this case, the acquisition is
 * started, with the resulting data flow processed then transferred to subscribers.
 */
acq_module_actor::behavior_type acquisition_actor_behavior(
    acq_module_actor::pointer self);

}  // namespace acq_module

#endif  // ACQUISITIONMODULEACTOR_HPP
