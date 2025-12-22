/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#include "DomainModelActor.hpp"

namespace domain_model
{

domain_model_actor_state::domain_model_actor_state(domain_model_actor::pointer_view self)
    : _self(self), _model(std::make_unique<domain_model::DomainModel>()){};

// --------------------------------------------------------------------

domain_model_actor::behavior_type domain_model_actor_state::make_behavior()
{
	return {[this](caf::publish_atom, int x) { _model->storeData(x); }};
};

}  // namespace domain_model
