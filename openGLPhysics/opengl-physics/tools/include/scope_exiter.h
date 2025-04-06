#pragma once

#include "config.h"


class ScopeExiter
{
public:
	ScopeExiter();

	ScopeExiter(std::function<void()> func);

	void set_exit_function(std::function<void()> func);

	~ScopeExiter();


private:
	std::function<void()> exitFunc; 
};

