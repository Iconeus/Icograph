/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef WORKFLOWACTOR_HPP
#define WORKFLOWACTOR_HPP

#include <caf/result.hpp>
#include <caf/type_list.hpp>
#include <caf/typed_actor.hpp>
#include <caf/typed_actor_pointer.hpp>

#include "Workflow.hpp"
#include "WorkflowType.hpp"
#include "WorkflowTypeIds.hpp"

#include "AcquisitionModuleActor.hpp"

namespace workflow
{

// Definition of the messaging interface of the workflow manager necessary to
// create the statically typed actor.
// /!\ CAF requires the argument to be written without the cv qualifiers.
struct workflow_trait
{
	using signatures = caf::type_list<caf::result<WorkflowType>(caf::get_atom),
	                                  caf::result<void>(init_workflow)>;
};

// Definition of the statically typed actor
using workflow_actor = caf::typed_actor<workflow_trait>;

/**
 * \struct workflow_actor_state
 *
 * @brief State class of the actor. Initialize the state of the actor and
 * provides the messaging behavior.
 */
struct workflow_actor_state
{
	/**
	 * @brief: Ctor
	 * @param: pointer to current actor
	 */
	workflow_actor_state(workflow_actor::pointer_view self, WorkflowType initialType);

	/**
	 * @brief: Defines the callbacks upon message reception
	 */
	workflow_actor::behavior_type make_behavior();

private:
	// Ptr to current actor
	workflow_actor::pointer_view _self;

	// Ptr to workflow implementation
	std::unique_ptr<Workflow> _currentWorkflow;
};

}  // namespace workflow

#endif  // WORKFLOWACTOR_HPP
