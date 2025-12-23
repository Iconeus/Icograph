/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef DOMAINMODEL_DOMAINMODEL_HPP
#define DOMAINMODEL_DOMAINMODEL_HPP

namespace domain_model
{

/**
 * \class DomainModel
 *
 * @brief To be filled once really implemented
 */
class DomainModel
{
public:
	DomainModel() = default;
	~DomainModel() = default;

	// Copy operations not allowed
	DomainModel(const DomainModel&) = delete;
	DomainModel& operator=(const DomainModel&) = delete;

	DomainModel(DomainModel&&) = default;
	DomainModel& operator=(DomainModel&&) = default;

	void storeData(int x);
};

}  // namespace domain_model

#endif /* DOMAINMODEL_DOMAINMODEL_HPP */
