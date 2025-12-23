/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef ECHOVIEWMODEL_ECHOVIEWER_HPP
#define ECHOVIEWMODEL_ECHOVIEWER_HPP

namespace echo_viewer_model
{

/**
 * \class EchoViewer
 *
 * @brief To be filled once really implemented
 */
class EchoViewer
{
public:
	EchoViewer() = default;
	~EchoViewer() = default;

	// Copy operations not allowed
	EchoViewer(const EchoViewer&) = delete;
	EchoViewer& operator=(const EchoViewer&) = delete;

	EchoViewer(EchoViewer&&) = default;
	EchoViewer& operator=(EchoViewer&&) = default;

	void displayFrame(int x);
};

}  // namespace echo_viewer_model

#endif /* ECHOVIEWMODEL_ECHOVIEWER_HPP */
