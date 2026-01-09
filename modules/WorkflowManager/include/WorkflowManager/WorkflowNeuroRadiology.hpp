/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef WORKFLOWMANAGER_WORKFLOWNEURORADIOLOGY_HPP
#define WORKFLOWMANAGER_WORKFLOWNEURORADIOLOGY_HPP

#include "Workflow.hpp"
#include "WorkflowType.hpp"

namespace workflow
{

/**
 * \class WorkflowNeuroRadiology
 *
 * @brief Holder of the logic behind a neuro radiology workflow
 */
class WorkflowNeuroRadiology : public Workflow
{
public:
	// Ctor
	WorkflowNeuroRadiology() = default;

	// Dtor
	virtual ~WorkflowNeuroRadiology() = default;

	/**
	 * @brief Copy constructor (deleted).
	 * Copy operations are not allowed for workflows.
	 */
	WorkflowNeuroRadiology(const WorkflowNeuroRadiology&) = delete;

	/**
	 * @brief Copy assignment operator (deleted).
	 * Copy operations are not allowed for workflows.
	 */
	WorkflowNeuroRadiology& operator=(const WorkflowNeuroRadiology&) = delete;

	// Move ctor
	WorkflowNeuroRadiology(WorkflowNeuroRadiology&&) = default;

	// Move operator
	WorkflowNeuroRadiology& operator=(WorkflowNeuroRadiology&&) = default;

	/**
	 * @brief Executes the workflow specific for neonate.
	 */
	void execute() override;

	// Get the type of the workflow
	WorkflowType getType() const override;
};

}  // namespace workflow

#endif  // WORKFLOWMANAGER_WORKFLOWNEURORADIOLOGY_HPP
