#include "tools\include\invokable.h"



namespace tools
{
	template<class ...Args>
	inline void Invokable::change_parameters(Args ...args)
	{

		auto* derived = dynamic_cast<Invokable*>(this);

		static_assert(std::is_base_of<Invokable, decltype(derived)>::value, "The class must be derived from Invokable");
		
		if (!derived)
		{
			throw std::runtime_error("Invalid dynamic cast in change_parameters");
		}
		derived->change_parameter(std::forward<Args>(args)...);
	}
}


/* Attempted to use CRTP (Curiously Recurring Template Pattern) and concepts, but it didn't work out as expected.
* 
* 	template <typename T, typename... Args>
	concept HasChangeParameter = requires(T t, Args&&... args) {
		{
			t.change_parameter(std::forward<Args>(args)...)
		};
	};
namespace tools {

	template <typename Derived>
	class Invokable
	{
	public:

		Invokable() = default;

		//virtual bool requires_change() const = 0;

		virtual bool execute() const = 0;

		template <class ... Args>
			requires HasChangeParameter<Derived, Args...>
		void change_parameters(Args... args); // TO KNOW: This function throws an exception if the dynamic cast fails, which usually happens when the args list isn't matching the parameter of the specific function.

		bool get_result() const;

		void set_updater(std::function<bool()> updater);

		virtual ~Invokable() = default;

	protected:
		std::function<void()> _updater;
		mutable bool _result = false;
	};

}


namespace tools
{
	template<typename Derived>
	bool Invokable<Derived>::get_result() const
	{
		return _result;
	}
	template<typename Derived>
	void Invokable<Derived>::set_updater(std::function<bool()> updater)
	{
		_updater = std::move(updater);
	}

	template<typename Derived>
	template<class ...Args>
		requires HasChangeParameter<Derived, Args...>
	inline void Invokable<Derived>::change_parameters(Args ...args)
	{
		static_assert(std::is_base_of<Invokable, Derived>::value, "Derived must inherit from Invokable");

		auto* derived = dynamic_cast<Derived*>(this);
		if (!derived)
		{
			throw std::runtime_error("Invalid dynamic cast in change_parameters");
		}
		derived->change_parameter(std::forward<Args>(args)...);
	}




} // namespace tools

*/