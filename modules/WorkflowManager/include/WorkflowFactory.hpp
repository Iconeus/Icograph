/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef WORKFLOW_FACTORY_H
#define WORKFLOW_FACTORY_H

#include <memory>
#include <string>

#include "Workflow.hpp"
#include "WorkflowType.hpp"

namespace workflow
{

/**
 * \class WorkflowFactory
 *
 * @brief Factory for the workflows.
 */
class WorkflowFactory
{
public:
	/**
	 * @brief: Creates the workflow
	 * @param: type of the workflow
	 */
	static std::unique_ptr<Workflow> createWorkflow(WorkflowType type);
};

}  // namespace workflow

#endif  // WORKFLOW_FACTORY_H
