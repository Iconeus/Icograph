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

#include "WorkflowNeuroSurgery.hpp"

namespace workflow
{

void WorkflowNeuroSurgery::execute()
{
	std::cout << "Execute workflow received " << std::endl;
}

// --------------------------------------------------------------------

WorkflowType WorkflowNeuroSurgery::getType() const
{
	return WorkflowType::NeuroSurgery;
}

}  // namespace workflow
