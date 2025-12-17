/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#include <caf/actor_registry.hpp>

#include "Workflow.hpp"
#include "WorkflowActor.hpp"
#include "WorkflowFactory.hpp"
#include "WorkflowType.hpp"
#include "WorkflowTypeIds.hpp"

#include "AcquisitionModuleActor.hpp"
#include "AcquisitionModuleTypeIds.hpp"

#include "CustomActorIdentifier.hpp"

namespace workflow
{

workflow_actor_state::workflow_actor_state(workflow_actor::pointer_view self,
                                           WorkflowType initialType)
    : _self(self)
{
	_currentWorkflow = WorkflowFactory::createWorkflow(initialType);
}

// --------------------------------------------------------------------

workflow_actor::behavior_type workflow_actor_state::make_behavior()
{
	return {
	    [this](caf::get_atom) { return _currentWorkflow->getType(); },
	    [this](init_workflow)
	    {
		    // Execute whatever the worklow manager needs to perform here (update the
		    // state, initialise objects, log etc)
		    _currentWorkflow->execute();

		    // Spawn acquisition module actor.
		    auto acquisitionModuleActorHandle =
		        _self->spawn(acq_module::acquisition_actor_behavior);

		    // Retrieve the actor that should receive the result of the acquisition. Here
		    // the domain model for storage and the echo viewer for display.
		    caf::actor_registry& registry = _self->system().registry();

		    std::vector<caf::actor> destActors{
		        registry.get<caf::actor>(common_caf::custom_echo_viewer_actor_id),
		        registry.get<caf::actor>(common_caf::custom_domain_model_actor_id)};

		    // Send start acquisition message with acquisition parameters and destinatory
		    // actors
		    _self->mail(acq_request_v, int32_t{42}, std::vector<caf::actor>{destActors})
		        .send(acquisitionModuleActorHandle);
	    }};
};

}  // namespace workflow
