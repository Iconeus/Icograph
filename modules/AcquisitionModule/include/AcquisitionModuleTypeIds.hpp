/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef ICOGRAPH_ACQUISITION_TYPE_IDS_HPP
#define ICOGRAPH_ACQUISITION_TYPE_IDS_HPP

#include <caf/type_id.hpp>

#include "CustomMessageIdentifier.hpp"

// Creates custom message types for the acquisition module
CAF_BEGIN_TYPE_ID_BLOCK(custom_types_acq_module, common_caf::custom_types_acq_module_id)
CAF_ADD_ATOM(custom_types_acq_module, acq_request)
CAF_END_TYPE_ID_BLOCK(custom_types_acq_module)

#endif  // ICOGRAPH_ACQUISITION_TYPE_IDS_HPP
