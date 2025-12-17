/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef WORKFLOW_NEURORADIOLOGY_H
#define WORKFLOW_NEURORADIOLOGY_H

#include "Workflow.hpp"
#include "WorkflowType.hpp"

namespace workflow
{

/**
 * \class WorkflowNeuroradiology
 *
 * @brief Holder of the logic behind a neuro radiology workflow
 */
class WorkflowNeuroradiology : public Workflow
{
public:
	// Ctor
	WorkflowNeuroradiology() = default;

	// Dtor
	virtual ~WorkflowNeuroradiology() = default;

	/**
	 * @brief Copy constructor (deleted).
	 * Copy operations are not allowed for workflows.
	 */
	WorkflowNeuroradiology(const WorkflowNeuroradiology&) = delete;

	/**
	 * @brief Copy assignment operator (deleted).
	 * Copy operations are not allowed for workflows.
	 */
	WorkflowNeuroradiology& operator=(const WorkflowNeuroradiology&) = delete;

	// Move ctor
	WorkflowNeuroradiology(WorkflowNeuroradiology&&) = default;

	// Move operator
	WorkflowNeuroradiology& operator=(WorkflowNeuroradiology&&) = default;

	/**
	 * @brief Executes the workflow specific for neonate.
	 */
	void execute() override;

	// Get the name of the workflow
	std::string getName() const override;

	// Get the type of the workflow
	WorkflowType getType() const override;
};

}  // namespace workflow

#endif  // WORKFLOW_NEURORADIOLOGY_H
