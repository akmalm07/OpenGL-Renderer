#include "headers.h"

#if 0

#include "tools\include\mouse_control.h"


namespace tools
{


	MouseControl::MouseControl() = default;

	double MouseControl::GetMouseCurrentX() const
	{
		return _mouseCurrentX;
	}

	double MouseControl::GetMouseCurrentY() const
	{
		return _mouseCurrentY;
	}

	double MouseControl::GetMouseChangeX() const
	{
		return _mouseChangeX;
	}

	double MouseControl::GetMouseChangeY() const
	{
		return _mouseChangeY;
	}

	float MouseControl::GetMouseCurrentXf() const
	{
		return (float)_mouseCurrentX;
	}

	float MouseControl::GetMouseCurrentYf() const
	{
		return (float)_mouseCurrentY;
	}

	float MouseControl::GetMouseChangeXf() const
	{
		return (float)_mouseChangeX;
	}

	float MouseControl::GetMouseChangeYf() const
	{
		return (float)_mouseChangeY;
	}


	bool MouseControl::IsMouseButtonPressed() const
	{
		return _isMouseButtonPressed;
	}


	void MouseControl::SetMouseChangeUpdater(std::function<bool()> func)
	{
		if (_mouseMove)
		{
			_mouseMove->set_updater(std::move(func));
		}
	}

	bool MouseControl::IsFirstClick() const
	{
		return _isFirstClick;
	}

	std::shared_ptr<MouseMovementB> MouseControl::GetMouseMove() const
	{
		return _mouseMove;
	}
}

#endif