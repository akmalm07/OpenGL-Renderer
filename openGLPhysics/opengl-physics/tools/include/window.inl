#pragma once

#include "tools/include/window.h"


namespace tools
{
	/*

	inline WindowT<ProjectionType::Ortho, CameraType::Classic>::WindowT(float windowWidth, float windowHeight, const std::string& name, bool createWindow, const CameraBundleOrthographic& camBundle)
		: ClassicWindow(windowWidth, windowHeight, name, createWindow)
	{
		_camera = std::make_shared<Camera>(camBundle);
	}

	inline WindowT<ProjectionType::Ortho, CameraType::Quaternion>::WindowT(float windowWidth, float windowHeight, const std::string& name, bool createWindow, const CameraBundleOrthographic& camBundle)
		: QuatWindow(windowWidth, windowHeight, name, createWindow)
	{
		_camera = std::make_shared<QuaternionCamera>(camBundle);
	}

	inline WindowT<ProjectionType::Persp, CameraType::Classic>::WindowT(float windowWidth, float windowHeight, const std::string& name, bool createWindow, const CameraBundlePerspective& camBundle)
		: ClassicWindow(windowWidth, windowHeight, name, createWindow)
	{
		_camera = std::make_shared<Camera>(camBundle);
	}

	inline WindowT<ProjectionType::Persp, CameraType::Quaternion>::WindowT(float windowWidth, float windowHeight, const std::string& name, bool createWindow, const CameraBundleOrthographic& camBundle)
		: QuatWindow(windowWidth, windowHeight, name, createWindow)
	{
		_camera = std::make_shared<QuaternionCamera>(camBundle);
	}



	inline WindowT<ProjectionType::Ortho, CameraType::Classic>::WindowT(WindowT&& other) noexcept
		: ClassicWindow(std::move(other))
	{
	}

	inline WindowT<ProjectionType::Ortho, CameraType::Classic>& WindowT<ProjectionType::Ortho, CameraType::Classic>::operator=(WindowT&& other) noexcept
	{
		if (this != &other)
		{
			ClassicWindow::operator=(std::move(other));
		}
		return *this;
	}

	inline WindowT<ProjectionType::Ortho, CameraType::Classic>& WindowT<ProjectionType::Ortho, CameraType::Classic>::operator=(WindowT&& other) noexcept
	{
		if (this != &other)
		{
			ClassicWindow::operator=(std::move(other));
		}
		return *this;
	}

	inline WindowT<ProjectionType::Ortho, CameraType::Classic>& WindowT<ProjectionType::Persp, CameraType::Quaternion>::operator=(WindowT&& other) noexcept
	{
		if (this != &other)
		{
			QuatWindow::operator=(std::move(other));
		}
		return *this;
	}

	inline WindowT<ProjectionType::Ortho, CameraType::Classic>& WindowT<ProjectionType::Persp, CameraType::Quaternion>::operator=(WindowT&& other) noexcept
	{
		if (this != &other)
		{
			QuatWindow::operator=(std::move(other));
		}
		return *this;
	}

	inline WindowT<ProjectionType::Ortho, CameraType::Quaternion>::WindowT(WindowT&& other) noexcept
		: QuatWindow(std::move(other))
	{
	}

	inline WindowT<ProjectionType::Persp, CameraType::Classic>::WindowT(WindowT&& other) noexcept
		: ClassicWindow(std::move(other))
	{
	}

	inline WindowT<ProjectionType::Persp, CameraType::Quaternion>::WindowT(WindowT&& other) noexcept
		: QuatWindow(std::move(other))
	{
	}

	*/

}