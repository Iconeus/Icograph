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

#include "WorkflowNeuroradiology.hpp"

namespace workflow
{

void WorkflowNeuroradiology::execute()
{
	std::cout << "Execute workflow received " << std::endl;
}

// --------------------------------------------------------------------

std::string WorkflowNeuroradiology::getName() const
{
	return to_string(getType());
}

// --------------------------------------------------------------------

WorkflowType WorkflowNeuroradiology::getType() const
{
	return WorkflowType::NEURORADIOLOGY;
}

}  // namespace workflow
