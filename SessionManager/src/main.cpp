/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#include <iostream>
#include <string>

#include <caf/actor_system.hpp>
#include <caf/caf_main.hpp>

#include "SessionManager.hpp"

#include "AcquisitionModuleTypeIds.hpp"
#include "WorkflowTypeIds.hpp"

int caf_main(caf::actor_system& system)
{
	std::cout << "C++ version: " << __cplusplus << std::endl;

	session_manager::SessionManager sessionManager(system);

	system.await_all_actors_done();

	return 0;
}

// Used defined ID must be specified here
CAF_MAIN(caf::id_block::custom_types_workflow, caf::id_block::custom_types_acq_module)
