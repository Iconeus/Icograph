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

#include "Logger/Logger.hpp"

#include "SessionManager/SessionManager.hpp"

#include "AcquisitionModule/AcquisitionModuleTypeIds.hpp"
#include "WorkflowManager/WorkflowTypeIds.hpp"

int caf_main(caf::actor_system& system)
{
	try
	{
		using namespace std::string_literals;

		// Instantiate the logger with the input configuration.
		// Will read the configuration from a file in the future (during installation and
		// deployment development).

		// Logger is valid as long as
		medlog::Logger logger(medlog::LoggerConfig{.app_name = "SessionManager"s,
		                                           .log_filename = L"SessionManager.log"s,
		                                           .level = medlog::LogLevel::Info});

		session_manager::SessionManager sessionManager(system);

		system.await_all_actors_done();

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << "unhandled exception: " << e.what() << "\n";
		return -1;
	}
}

// Used defined ID must be specified here
CAF_MAIN(caf::id_block::custom_types_workflow, caf::id_block::custom_types_acq_module)
