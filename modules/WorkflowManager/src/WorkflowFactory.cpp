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
#include "WorkflowNeurosurgery.hpp"

namespace workflow
{

std::unique_ptr<Workflow> WorkflowFactory::createWorkflow(WorkflowType type)
{
	std::unique_ptr<Workflow> ptr{nullptr};
	switch (type)
	{
	case WorkflowType::Neonate:
		ptr = std::make_unique<WorkflowNeonate>();
		break;
	case WorkflowType::NeuroRadiology:
		ptr = std::make_unique<WorkflowNeuroradiology>();
		break;
	case WorkflowType::NeuroSurgery:
		ptr = std::make_unique<WorkflowNeurosurgery>();
		break;
	}

	return ptr;
}

}  // namespace workflow
