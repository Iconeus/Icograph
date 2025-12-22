/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef ICOGRAPH_ECHOVIEWERACTOR_HPP
#define ICOGRAPH_ECHOVIEWERACTOR_HPP

#include <caf/result.hpp>
#include <caf/type_list.hpp>
#include <caf/typed_actor.hpp>
#include <caf/typed_actor_pointer.hpp>

#include "EchoViewer.hpp"

namespace echo_viewer
{

// Definition of the messaging interface of the echo viewer necessary to
// create the statically typed actor.
// /!\ CAF requires the argument to be written without the cv qualifiers.
struct echo_viewer_trait
{
	using signatures = caf::type_list<caf::result<void>(caf::publish_atom, int)>;
};

// Definition of the statically typed actor
using echo_viewer_actor = caf::typed_actor<echo_viewer_trait>;

/**
 * \class echo_viewer_actor_state
 *
 * @brief State class of the actor. Initialize the state of the actor and
 * provides the messaging behavior.
 */
class echo_viewer_actor_state
{
public:
	/**
	 * @brief: Ctor
	 * @param: pointer to current actor
	 */
	echo_viewer_actor_state(echo_viewer_actor::pointer_view self);

	/**
	 * @brief: Defines the callbacks upon message reception
	 */
	echo_viewer_actor::behavior_type make_behavior();

private:
	// Ptr to current actor
	echo_viewer_actor::pointer_view _self;

	// Ptr to workflow implementation
	std::unique_ptr<echo_viewer::EchoViewer> _viewer{nullptr};
};

}  // namespace echo_viewer

#endif  // ICOGRAPH_ECHOVIEWERACTOR_HPP
