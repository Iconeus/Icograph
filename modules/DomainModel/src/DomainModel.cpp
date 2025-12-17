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

#include "DomainModel.hpp"

namespace domain_model
{

void DomainModel::storeData(int x)
{
	std::cout << "Storing frame: " << std::to_string(x) << std::endl;
}

}  // namespace domain_model
