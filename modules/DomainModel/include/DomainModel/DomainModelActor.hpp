/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef DOMAINMODEL_DOMAINMODELACTOR_HPP
#define DOMAINMODEL_DOMAINMODELACTOR_HPP

#include <caf/result.hpp>
#include <caf/type_list.hpp>
#include <caf/typed_actor.hpp>
#include <caf/typed_actor_pointer.hpp>

#include "DomainModel.hpp"

namespace domain_model
{

// Definition of the messaging interface of the domain model necessary to
// create the statically typed actor.
// /!\ CAF requires the argument to be written without the cv qualifiers.
struct domain_model_trait
{
	using signatures = caf::type_list<caf::result<void>(caf::publish_atom, int)>;
};

// Definition of the statically typed actor
using domain_model_actor = caf::typed_actor<domain_model_trait>;

/**
 * \class domain_model_actor_state
 *
 * @brief State class of the actor. Initialize the state of the actor and
 * provides the messaging behavior.
 */
class domain_model_actor_state
{
public:
	/**
	 * @brief: Ctor
	 * @param: pointer to current actor
	 */
	explicit domain_model_actor_state(domain_model_actor::pointer_view self);

	/**
	 * @brief: Defines the callbacks upon message reception
	 */
	domain_model_actor::behavior_type make_behavior();

private:
	// Ptr to current actor
	domain_model_actor::pointer_view _self;

	// Ptr to workflow implementation
	std::unique_ptr<domain_model::DomainModel> _model{nullptr};
};

}  // namespace domain_model

#endif  // DOMAINMODEL_DOMAINMODELACTOR_HPP
