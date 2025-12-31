/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#include <algorithm>
#include <execution>
#include <vector>

#include <caf/actor_registry.hpp>
#include <caf/event_based_actor.hpp>
#include <caf/scheduled_actor/flow.hpp>
#include <caf/type_id.hpp>

#include "AcquisitionModule/AcquisitionModule.hpp"
#include "AcquisitionModule/AcquisitionModuleActor.hpp"
#include "AcquisitionModule/AcquisitionModuleTypeIds.hpp"
#include "CAF/CustomActorIdentifier.hpp"

namespace acq_module
{

/**
 * @brief Creates a behavior that generates a stream of data (to be changed later by the
 * acquisition data. For now it returns a stream of integers called "int-flow" ). It shall
 * be used as a source of data for other actors that consume streams.
 *
 * @param self The current actor
 *
 * @return A `caf::behavior` that handles the `caf::get_atom` message.
 */
static caf::behavior sourceFun(caf::event_based_actor* self)
{
	return {[self](caf::get_atom)
	        {
		        return self->make_observable().iota(1).take(5).to_stream(
		            "int-flow", caf::defaults::stream::max_batch_delay, 100);
	        }};
}

// --------------------------------------------------------------------
/**
 * @brief Consumes a stream from a source actor and forwards each item to one or more
 * destination actors.
 *
 * @param self The current actor (must be an `caf::event_based_actor`).
 * @param src The source actor from which the stream is requested.
 * @param dests One or more destination actors to which each item of the stream will be
 * forwarded.
 *
 * @return An empty behavior, as all logic is handled asynchronously.
 */
static caf::behavior consumeFun(caf::event_based_actor* self,
                                caf::actor src,
                                std::vector<caf::actor> destActors)
{
	self->mail(caf::get_atom_v)
	    .request(src, caf::infinite)
	    .then(
	        [=](caf::stream s)
	        {
		        // Turn the stream handle into an observable, then consume it.
		        self->observe_as<int>(s, 50u, 10u)
		            .for_each(
		                [=](int x)
		                {
			                // Send each item of the stream to the destinatory actors
			                std::for_each(
			                    std::execution::par, destActors.begin(), destActors.end(),
			                    [self, x](caf::actor destActor)
			                    { self->mail(caf::publish_atom_v, x).send(destActor); });
		                });
	        },
	        [=](caf::error& err) { self->println("get_atom failed: {}", err); });

	return {};
}

// --------------------------------------------------------------------
acq_module_actor::behavior_type acquisition_actor_behavior(acq_module_actor::pointer self)
{
	return {
	    [self](acq_request, int32_t parameterValue, std::vector<caf::actor> destActors)
	    {
		    // Handle acquisition request (one day it will be a call to Moduleus
		    // facade)
		    AcquisitionModule acqModule;
		    acqModule.acquisitionRequest(parameterValue);

		    // Producer: Actor in which the flow of acquisition data will pass (this
		    // is a theory that will be conformed in following work during Moduleus's
		    // simulator creation).
		    // The reason we use caf::stream and not plain caf::observable is because
		    // observables cannot be read simultaneously by several observers at the
		    // same time.
		    auto srcActor = self->spawn(sourceFun);

		    // Consumer: Will create an observer for the stream of data and will pass
		    // the result to the viewer actor
		    auto consumerActor = self->spawn(consumeFun, srcActor, destActors);
	    }};
}

}  // namespace acq_module
