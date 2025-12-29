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

#include "Logger/Logger.hpp"

#include "EchoViewModel/EchoViewer.hpp"

namespace echo_view_model
{

void EchoViewer::displayFrame(int x)
{
	medlog::info("Display frame {}", x);
}

}  // namespace echo_view_model
