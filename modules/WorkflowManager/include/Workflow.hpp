/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef WORKFLOW_H
#define WORKFLOW_H

#include "WorkflowType.hpp"

namespace workflow
{

/**
 * \class Workflow
 *
 * @brief Base class for the workflows. This class handles the specialization behavior
 * between the different types of workflows (Neonate, Neuroradiology etc). It holds a
 * state machine that guides the session through the different workflow steps.
 */
class Workflow
{
public:
	// Ctor
	Workflow() = default;

	// Dtor
	virtual ~Workflow() = default;

	/**
	 * @brief Copy constructor (deleted).
	 * Copy operations are not allowed for workflows.
	 */
	Workflow(const Workflow&) = delete;

	/**
	 * @brief Copy assignment operator (deleted).
	 * Copy operations are not allowed for workflows.
	 */
	Workflow& operator=(const Workflow&) = delete;

	// Move ctor
	Workflow(Workflow&&) = default;

	// Move operator
	Workflow& operator=(Workflow&&) = default;

	/**
	 * @brief Executes the workflow.
	 * This method should be overridden by derived classes to implement workflow-specific
	 * logic.
	 */
	virtual void execute() = 0;

	// Get the name of the workflow
	virtual std::string getName() const = 0;

	// Get the type of the workflow
	virtual WorkflowType getType() const = 0;
};

}  // namespace workflow

#endif  // WORKFLOW_H
