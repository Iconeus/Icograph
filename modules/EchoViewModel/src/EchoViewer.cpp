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

#include "EchoViewer.hpp"

namespace echo_viewer
{

void EchoViewer::displayFrame(int x)
{
	std::cout << "Displaying frame: " << std::to_string(x) << std::endl;
}

}  // namespace echo_viewer
