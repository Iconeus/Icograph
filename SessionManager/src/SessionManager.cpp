/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#include "SessionManager.hpp"

#include "CustomActorIdentifier.hpp"
#include "DomainModelActor.hpp"
#include "EchoViewerActor.hpp"
#include "WorkflowActor.hpp"

#include <caf/actor_from_state.hpp>
#include <caf/actor_registry.hpp>
#include <caf/event_based_actor.hpp>

namespace session_manager
{

static void callWorkflowActor(caf::event_based_actor* self,
                              const workflow::workflow_actor& workflowActor)
{
	self->mail(init_workflow_v).send(workflowActor);

	self->mail(caf::get_atom_v)
	    .request(workflowActor, std::chrono::seconds(1))
	    .then(
	        // ... wait up to 1s for a response ...
	        [self](workflow::WorkflowType workflowType)
	        {
		        // ... and print it
		        self->println("Workflow type received {}",
		                      workflow::to_string(workflowType));
	        });
}

// --------------------------------------------------------------------

SessionManager::SessionManager(caf::actor_system& system)
{
	// Spawn acquisition view model actor.
	// STATEFUL to keep the state of the display.
	// Will be created with Qt Quick context (main Qt thread handling coming afterwards)
	auto echoViewerActorHandle =
	    system.spawn(caf::actor_from_state<echo_viewer::echo_viewer_actor_state>);

	// Spawn domain Model actor.
	// STATEFUL to store in-memory caching of the data and the list of the data
	// related to the current patient.
	auto domainModelActorHandle =
	    system.spawn(caf::actor_from_state<domain_model::domain_model_actor_state>);

	// Spawn workflow actor.
	// STATEFUL to keep the current state of the acquisition workflow
	// Contains a state machine that drives the workflow steps
	auto workflowManagerActorHandle =
	    system.spawn(caf::actor_from_state<workflow::workflow_actor_state>,
	                 workflow::WorkflowType::Neonate);

	// Map actors with an ID in the actor system registry to make them system-wide
	// available by any other actor within the same actor system.
	system.registry().put(common_caf::custom_echo_viewer_actor_id, echoViewerActorHandle);
	system.registry().put(common_caf::custom_domain_model_actor_id,
	                      domainModelActorHandle);
	system.registry().put(common_caf::custom_workflow_manager_actor_id,
	                      workflowManagerActorHandle);

	system.spawn(callWorkflowActor, workflowManagerActorHandle);
}

}  // namespace session_manager
