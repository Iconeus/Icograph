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
#include "WorkflowManager/WorkflowNeonate.hpp"

namespace workflow
{

void WorkflowNeonate::execute()
{
	medlog::info("Execute workflow received");
}

// --------------------------------------------------------------------

WorkflowType WorkflowNeonate::getType() const
{
	return WorkflowType::Neonate;
}

}  // namespace workflow
