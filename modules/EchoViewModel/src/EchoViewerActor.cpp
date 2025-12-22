/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#include "EchoViewerActor.hpp"

namespace echo_viewer
{

echo_viewer_actor_state::echo_viewer_actor_state(echo_viewer_actor::pointer_view self)
    : _self(self), _viewer(std::make_unique<echo_viewer::EchoViewer>()){};

// --------------------------------------------------------------------

echo_viewer_actor::behavior_type echo_viewer_actor_state::make_behavior()
{
	return {[this](caf::publish_atom, int x) { _viewer->displayFrame(x); }};
};

}  // namespace echo_viewer
