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
#include "WorkflowManager/WorkflowNeuroRadiology.hpp"

namespace workflow
{

void WorkflowNeuroRadiology::execute()
{
	MEDLOG_INFO("Execute workflow received.");
}

// --------------------------------------------------------------------

WorkflowType WorkflowNeuroRadiology::getType() const
{
	return WorkflowType::NeuroRadiology;
}

}  // namespace workflow
