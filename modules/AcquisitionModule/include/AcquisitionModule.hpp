/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef ICOGRAPH_ACQUISITIONMODULE_HPP
#define ICOGRAPH_ACQUISITIONMODULE_HPP

#include <stdint.h>

namespace acq_module
{

/**
 * \class AcquisitionModule
 *
 * @brief TODO (during implementation of Moduleus simulator)
 */
class AcquisitionModule
{
public:
	// Ctor
	AcquisitionModule() = default;

	// Dtor
	~AcquisitionModule() = default;

	/**
	 * @brief: handler for the acquisition request
	 * @param: dummy value
	 */
	void acquisitionRequest(int32_t parameterValue);
};
}  // namespace acq_module

#endif  // ICOGRAPH_ACQUISITIONMODULE_HPP
