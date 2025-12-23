/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef WORKFLOWMANAGER_WORKFLOWNEONATE_HPP
#define WORKFLOWMANAGER_WORKFLOWNEONATE_HPP

#include "Workflow.hpp"
#include "WorkflowType.hpp"

namespace workflow
{

/**
 * \class WorkflowNeonate
 *
 * @brief Holder of the logic behind a neonate workflow
 */
class WorkflowNeonate : public Workflow
{
public:
	// Ctor
	WorkflowNeonate() = default;

	// Dtor
	virtual ~WorkflowNeonate() = default;

	/**
	 * @brief Copy constructor (deleted).
	 * Copy operations are not allowed for workflows.
	 */
	WorkflowNeonate(const WorkflowNeonate&) = delete;

	/**
	 * @brief Copy assignment operator (deleted).
	 * Copy operations are not allowed for workflows.
	 */
	WorkflowNeonate& operator=(const WorkflowNeonate&) = delete;

	// Move ctor
	WorkflowNeonate(WorkflowNeonate&&) = default;

	// Move operator
	WorkflowNeonate& operator=(WorkflowNeonate&&) = default;

	/**
	 * @brief Executes the workflow specific for neonate.
	 */
	void execute() override;

	// Get the type of the workflow
	WorkflowType getType() const override;
};

}  // namespace workflow

#endif  // WORKFLOWMANAGER_WORKFLOWNEONATE_HPP
