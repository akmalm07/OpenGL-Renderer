#pragma once

#include "config.h"


namespace tools
{


	class Invokable
	{
	public:

		Invokable() = default;


		virtual bool execute() const = 0;

		template <class ... Args>
		void change_parameters(Args... args);

		bool get_result() const;

		void set_updater(std::function<bool()>& updater);

		virtual ~Invokable() = default;

	protected:
		std::function<void()> _updater;
		mutable bool _result = false;
	};



}

#include "tools\include\invokable.inl"