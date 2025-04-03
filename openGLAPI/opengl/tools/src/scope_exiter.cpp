#include "headers.h"

#include "tools/include/scope_exiter.h"


ScopeExiter::ScopeExiter() = default;

ScopeExiter::ScopeExiter(std::function<void()> func)
	: exitFunc(std::move(func))
{
}

void ScopeExiter::set_exit_function(std::function<void()> func)
{
	exitFunc = std::move(func);
}


ScopeExiter::~ScopeExiter()
{
	if (exitFunc)
	{
		exitFunc();
	}
	else
	{
		std::cerr << "Warning: ScopeExiter destructor called without a valid exit function!" << std::endl;
	}
}

