#include "headers.h"

#include "tools\include\aabb_button_control.h"



namespace tools
{

	AABButtonControl::AABButtonControl()
	{
		_keys.fill(false);
	}


	void AABButtonControl::DelAABButton(std::string_view name)
	{
		for (auto& item : _AABButtons)
		{
			auto it = item.find(name);
			if (it == item.end())
			{
				throw std::runtime_error("Entered invalid window name ( Window.cpp : DelWindowButton )");
				return;
			}
			item.erase(it);
		}

	}

	void AABButtonControl::AddFuncParamUpdaterButton(std::string_view name, std::function<bool()> function)
	{
		for (auto& item : _AABButtons)
		{
			auto it = item.find(name);
			if (it != item.end())
			{
				it->second->set_updater(std::move(function));
				return;
			}
		}

		throw std::runtime_error("Entered invalid window name ( Window.cpp : AddWindowButton )");
	}


	void AABButtonControl::ChangeFuncParamUpdaterButton(std::string_view name, std::function<bool()> function)
	{
		AddFuncParamUpdaterButton(name, std::move(function));
	}


	std::shared_ptr<AABButtonB> AABButtonControl::FindAABButton(std::string_view name)
	{
		for (auto& item : _AABButtons)
		{
			for (auto& [key, val] : item)
			{
				if (key == name)
				{
					return val;
				}
			}
		}
		throw std::runtime_error("Could not find the AABButton with the name: " + std::string(name));
	}

	void AABButtonControl::SetKey(unsigned int key, bool val)
	{
		_keys[key] = val;
	}

}