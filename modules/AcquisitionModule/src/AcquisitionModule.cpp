/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#include <iostream>

#include "Logger/Logger.hpp"

#include "AcquisitionModule/AcquisitionModule.hpp"

namespace acq_module
{

void AcquisitionModule::acquisitionRequest(int32_t parameterValue)
{
	medlog::info("Acquisition request received with value: {}", parameterValue);
}

}  // namespace acq_module
