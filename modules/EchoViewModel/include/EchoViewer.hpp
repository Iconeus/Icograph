/*
 * Copyright © 2025 Iconeus. All rights reserved.
 *
 * This software is the proprietary and confidential property of Iconeus.
 * Any use, reproduction, modification or distribution without prior permission
 * is strictly prohibited.
 *
 * Author: Alyson Roger <alyson.roger@iconeus.com>
 */

#ifndef ECHOVIEWER_HPP
#define ECHOVIEWER_HPP

namespace echo_viewer
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

}  // namespace echo_viewer

#endif  // ECHOVIEWER_HPP
