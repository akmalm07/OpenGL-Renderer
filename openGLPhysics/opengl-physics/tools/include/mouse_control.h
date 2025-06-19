#pragma once

#if 0

#include "config.h"
#include "tools\include\keys.h"
#include "tools\include\window_input.h"


namespace tools
{

	class MouseControl
	{
	public:

		MouseControl();


		double GetMouseCurrentX() const;

		double GetMouseCurrentY() const;

		float GetMouseCurrentXf() const;

		float GetMouseCurrentYf() const;


		double GetMouseChangeX() const;

		double GetMouseChangeY() const;

		float GetMouseChangeXf() const;

		float GetMouseChangeYf() const;


		bool IsMouseButtonPressed() const;


		template<class ... Args>
		void AddMouseChange(const MouseMoveInput& mouse, std::function<bool(Args...)> func, Args&&... args);


		void SetMouseChangeUpdater(std::function<bool()> func);


		bool IsFirstClick() const;

		
		std::shared_ptr<MouseMovementB> GetMouseMove() const;

	protected:

		bool _isFirstClick = false;

		double _mouseChangeX = 0.0;
		double _mouseChangeY = 0.0;

		double _mouseCurrentX = 0.0;
		double _mouseCurrentY = 0.0;

		bool _mouseFirstMoved = true;

		bool _isMouseButtonPressed = false;

		std::shared_ptr<MouseMovementB> _mouseMove;
	};
	

	template<class ...Args>
	inline void MouseControl::AddMouseChange(const MouseMoveInput& mouse, std::function<bool(Args...)> func, Args && ...args)
	{
		_mouseMove = std::make_shared<MouseMovement<Args...>>(mouse, std::move(func), std::forward<Args>(args)...);
	}

}


#endif