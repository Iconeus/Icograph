/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#include "WorkflowFactory.hpp"
#include "WorkflowNeonate.hpp"
#include "WorkflowNeuroradiology.hpp"

namespace workflow
{

std::unique_ptr<Workflow> WorkflowFactory::createWorkflow(WorkflowType type)
{
	switch (type)
	{
	case WorkflowType::NEONATE:
		return std::make_unique<WorkflowNeonate>();
		break;
	case WorkflowType::NEURORADIOLOGY:
		return std::make_unique<WorkflowNeuroradiology>();
		break;
	default:
		return nullptr;  // Left as is until desired error management is chosen
	}
}

}  // namespace workflow
