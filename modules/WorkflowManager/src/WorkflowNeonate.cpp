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

#include "WorkflowNeonate.hpp"

namespace workflow
{

void WorkflowNeonate::execute()
{
	std::cout << "Execute workflow received " << std::endl;
}

// --------------------------------------------------------------------

WorkflowType WorkflowNeonate::getType() const
{
	return WorkflowType::Neonate;
}

}  // namespace workflow
