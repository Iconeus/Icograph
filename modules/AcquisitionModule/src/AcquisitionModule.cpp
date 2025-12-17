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

#include "AcquisitionModule.hpp"

namespace acq_module
{

void AcquisitionModule::acquisitionRequest(int32_t parameterValue)
{
	std::cout << "Acquisition request received with value: " << parameterValue
	          << std::endl;
}

}  // namespace acq_module
