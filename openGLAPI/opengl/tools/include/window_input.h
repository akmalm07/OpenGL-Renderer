#pragma once

#include "headers.h"

#include "tools\include\keys.h"
#include "tools\include\window_input_bundles.h"
#include "tools\include\thread.h"

#include <memory>
#include <condition_variable>


namespace tools 
{

	template<class ... Args> struct MouseButton;
	template<class ... Args> struct AABButton;
	template<class ... Args> struct KeyComb;
	template<class ... Args> struct MouseMovement;

	struct MouseButtonB
	{
	public:
		MouseButtonB();
		MouseButtonB(const MouseButtonInput& in);

		virtual bool requires_change() const = 0;

		virtual bool execute() const = 0;

		template <class ... Args>
		void change_parameters(Args... args)
		{
			auto* derived = dynamic_cast<MouseButton<Args...>*>(this);
			if (!derived)
			{
				throw std::runtime_error("Invalid dynamic cast in change_parameters");
			}
			derived->change_parameter(std::forward<Args>(args)...);
		}


		void set_updater(std::function<bool()> updater);

		bool getPressed() const;
		void setPressed(bool value);

		bool get_result() const;

		virtual ~MouseButtonB();

	protected:
		bool _pressed = false;
		Mouse _button;
		Action _action;
		std::function<bool()> _updater;
		mutable bool _result = false;
	};


	struct MouseMovementB
	{
	public:
		MouseMovementB();
		MouseMovementB(const MouseMoveInput& mouse);

		void set_updater(std::function<bool()> updater);

		virtual bool execute() const = 0;

		inline bool is_changed(float prevX, float prevY, float newX, float newY) const
		{
			if (_change == MouseChange::None)
				return false;
			return (prevX != newX || prevY != newY);
		}

		template <class ... Args>
		void change_parameters(Args... args)
		{
			auto* derived = dynamic_cast<MouseMovement<Args...>*>(this);
			if (!derived)
			{
				throw std::runtime_error("Invalid dynamic cast in change_parameters");
			}
			derived->change_parameter(std::forward<Args>(args)...);
		}

		bool get_result() const;

		virtual ~MouseMovementB();

	protected: 

		Mouse _button = Mouse::None;
		MouseChange _change = MouseChange::None;
		std::function<bool()> _updater;
		mutable bool _result = false;

	};

	struct AABButtonB
	{
	public:
		AABButtonB();
		AABButtonB(const AABButtonInput& in);

		virtual bool requires_change() const = 0;

		virtual bool execute() const = 0;

		bool is_clicked(float x, float y, Action action, Mouse button) const;

		void set_updater(std::function<bool()> updater);

		template <class ... Args>
		void change_parameters(Args... args)
		{
			auto* derived = dynamic_cast<AABButton<Args...>*>(this);
			if (!derived)
			{
				throw std::runtime_error("Invalid dynamic cast in change_parameters");
			}
			derived->change_parameter(std::forward<Args>(args)...);
		}

		bool get_result() const;

		virtual ~AABButtonB();

	protected:
		float _x, _y, _width, _height;
		Action _action;
		Mouse _button;
		std::string_view _name;
		std::function<bool()> _updater;
		mutable bool _result = false;
	};

	struct KeyCombB
	{
	public:
		KeyCombB();
			
		KeyCombB(const KeyCombInputOne& in);
		KeyCombB(const KeyCombInputPoly& in);

		virtual bool requires_change() const = 0;

		bool is_pressed(Keys number, Action action, Mods mod) const;
		
		bool is_pressed(int number, int mod) const;
		
		bool is_pressed(GLFWwindow* window, int mods) const;

		void set_updater(std::function<bool()> updater);

		virtual bool execute() const = 0;

		template <class ... Args>
		void change_parameters(Args... args)
		{
			auto* derived = dynamic_cast<KeyComb<Args...>*>(this);
			if (!derived)
			{
				throw std::runtime_error("Invalid dynamic cast in change_parameters");
			}
			derived->change_parameter(std::forward<Args>(args)...);
		}

		bool get_result() const;

		virtual ~KeyCombB();

	protected:
		std::array<Keys, KEY_MAX> _charater;
		std::optional<Mods> _mode;
		Action _trigger;
		std::function<bool()> _updater;
		mutable bool _result = false;

	};

	//----------------------------------------------
	// Templated Derived Classes for User Input

	template<class ... Args>
	struct AABButton : AABButtonB
	{
	public:
		AABButton(const AABButtonInput& in, std::function<bool(Args...)> func, Args&&... args)
			: AABButtonB(in), _func(std::move(func)), _args(std::forward<Args>(args)...) {
		}
		

		AABButton(const AABButtonInput& in, std::function<bool(Args...)> func, std::tuple<Args...>&& args)
			: AABButtonB(in), _func(std::move(func)), _args(std::forward<std::tuple<Args...>>(args)) {
		}

		bool execute() const override
		{
			if (_updater)
			{
				_updater();
			}
			_result = std::apply(_func, _args);
			return _result;
		}

		void change_parameter(Args&&... args)
		{
			_args = std::tuple<Args...>(std::forward<Args>(args)...);
		}

		~AABButton() override = default;

	private:
		std::function<bool(Args...)> _func;
		std::tuple<Args...> _args;
	};

	template<class ... Args>
	struct KeyComb : KeyCombB
	{
	public:
		KeyComb(const KeyCombInputOne& in, std::function<bool(Args...)> func, Args&&... args)
			: KeyCombB(in), _func(std::move(func)), _args(std::forward<Args>(args)...)
		{
		}

		KeyComb(const KeyCombInputOne& in, std::function<bool(Args...)> func, std::tuple<Args...>&& args)
			: KeyCombB(in), _func(std::move(func)), _args(std::forward<std::tuple<Args...>>(args))
		{
		}

		KeyComb(const KeyCombInputPoly& in, std::function<bool(Args...)> func, Args&&... args)
			: KeyCombB(in), _func(std::move(func)), _args(std::forward<Args>(args)...)
		{
		}

		KeyComb(const KeyCombInputPoly& in, std::function<bool(Args...)> func, std::tuple<Args...>&& args)
			: KeyCombB(in), _func(std::move(func)), _args(std::forward<std::tuple<Args...>>(args))
		{
		}


		std::function<bool(Args...)> get_func() const
		{
			return _func;
		}

		bool execute() const override
		{
			if (_updater)
			{
				_updater();
			}
			_result = std::apply(_func, _args);
			return _result;
		}

		bool requires_change() const override
		{
			if constexpr (std::tuple_size<std::tuple<Args...>>::value == 0)
			{
				return false;
			}
			return true;
		}

		void change_parameter(Args&&... args)
		{
			_args = std::tuple<Args...>(std::forward<Args>(args)...);
		}

		~KeyComb() override = default;

	private:
		std::function<bool(Args...)> _func;
		std::tuple<Args...> _args;
	};

	template<class ... Args>
	struct MouseButton : MouseButtonB
	{
	public:
		MouseButton(const MouseMoveInput& in, std::function<bool(Args...)> func, Args&&... args)
			: MouseButtonB(in), _func(std::move(func)), _args(std::forward<Args>(args)...) {
		}

		MouseButton(const MouseMoveInput& in, std::function<bool(Args...)> func, std::tuple<Args...>&& args)
			: MouseButtonB(in), _func(std::move(func)), _args(std::forward<std::tuple<Args...>>(args)) { //recent add
		}

		bool execute() const override
		{
			if (_updater)
			{
				_updater();
			}
			_result = std::apply(_func, _args);
			return _result;
		}

		bool requires_change() const override
		{
			if constexpr (std::tuple_size<std::tuple<Args...>>::value == 0)
			{
				return false;
			}
			return true;
		}

		void change_parameter(Args&&... args)
		{
			_args = std::tuple<Args...>(std::forward<Args>(args)...);
		}

		~MouseButton() override = default;

	private:
		std::function<bool(Args...)> _func;
		std::tuple<Args...> _args;
	};


	template<class ... Args>
	struct MouseMovement : MouseMovementB
	{
	public:
		MouseMovement(const MouseMoveInput& in, std::function<bool(Args...)> func, Args&&... args)
			: MouseMovementB(in), _func(std::move(func)), _args(std::forward<Args>(args)...) {
		}
		MouseMovement(const MouseMoveInput& in, std::function<bool(Args...)> func, std::tuple<Args...>&& args)
			: MouseMovementB(in), _func(std::move(func)), _args(std::forward<std::tuple<Args...>>(args)) { //recent add
		}
		bool execute() const override
		{
			if (_updater)
			{
				_updater();
			}
			_result = std::apply(_func, _args);
			return _result;
		}

		void change_parameter(Args&&... args)
		{
			_args = std::tuple<Args...>(std::forward<Args>(args)...);
		}
		~MouseMovement() override = default;
	private:
		std::function<bool(Args...)> _func;
		std::tuple<Args...> _args;
	};
}


//template <class ... Args>
//std::function<bool(Args...)> get_funcs()
//{
//	auto* derived = dynamic_cast<KeyComb<Args...>*>(this);
//	if (!derived)
//	{
//		throw std::runtime_error("Invalid dynamic cast in change_parameters");
//	}
//	return derived->get_func();
//}