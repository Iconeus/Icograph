/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef SESSIONMANAGER_HPP
#define SESSIONMANAGER_HPP

#include <caf/actor_system.hpp>

namespace session_manager
{

/**
 * \class SessionManager
 *
 * @brief Orchestrator and monitorer of the session, it generates and
 * coordinates all the actors of the application.
 */
class SessionManager
{
public:
	// Ctor
	SessionManager(caf::actor_system& system);

	// Dtor
	~SessionManager() = default;
};

}  // namespace session_manager

#endif  // SESSIONMANAGER_HPP
