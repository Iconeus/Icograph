/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef WORKFLOWMANAGER_WORKFLOW_NEUROSURGERY_H
#define WORKFLOWMANAGER_WORKFLOW_NEUROSURGERY_H

#include "Workflow.hpp"
#include "WorkflowType.hpp"

namespace workflow
{

/**
 * \class WorkflowNeuroSurgery
 *
 * @brief Holder of the logic behind a neuro radiology workflow
 */
class WorkflowNeuroSurgery : public Workflow
{
public:
	// Ctor
	WorkflowNeuroSurgery() = default;

	// Dtor
	virtual ~WorkflowNeuroSurgery() = default;

	/**
	 * @brief Copy constructor (deleted).
	 * Copy operations are not allowed for workflows.
	 */
	WorkflowNeuroSurgery(const WorkflowNeuroSurgery&) = delete;

	/**
	 * @brief Copy assignment operator (deleted).
	 * Copy operations are not allowed for workflows.
	 */
	WorkflowNeuroSurgery& operator=(const WorkflowNeuroSurgery&) = delete;

	// Move ctor
	WorkflowNeuroSurgery(WorkflowNeuroSurgery&&) = default;

	// Move operator
	WorkflowNeuroSurgery& operator=(WorkflowNeuroSurgery&&) = default;

	/**
	 * @brief Executes the workflow specific for neonate.
	 */
	void execute() override;

	// Get the type of the workflow
	WorkflowType getType() const override;
};

}  // namespace workflow

#endif  // WORKFLOWMANAGER_WORKFLOW_NEUROSURGERY_H
