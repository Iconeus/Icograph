/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef ICOGRAPH_WORKFLOW_TYPE_IDS_H
#define ICOGRAPH_WORKFLOW_TYPE_IDS_H

#include <caf/type_id.hpp>

#include "CustomMessageIdentifier.hpp"

#include "WorkflowType.hpp"

// Definition of custom types for workflow messages types
CAF_BEGIN_TYPE_ID_BLOCK(custom_types_workflow, common_caf::custom_types_workflow_id)
CAF_ADD_TYPE_ID(custom_types_workflow, (workflow::WorkflowType))
CAF_ADD_ATOM(custom_types_workflow, init_workflow)
CAF_END_TYPE_ID_BLOCK(custom_types_workflow)

namespace workflow
{

// An inspect function needs to be provided to CAF to be able to serialize the custom
// message type WorkflowType
template <class Inspector>
bool inspect(Inspector& f, workflow::WorkflowType& workflowType)
{
	return caf::default_enum_inspect(f, workflowType);
}

}  // namespace workflow

#endif  // ICOGRAPH_WORKFLOW_TYPE_IDS_H
