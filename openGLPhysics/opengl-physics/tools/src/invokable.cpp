#include "headers.h"
#include "tools/include/invokable.h"


namespace tools
{
    bool Invokable::get_result() const
    {
        return _result;
    }

    void Invokable::set_updater(std::function<bool()>& updater)
    {
		_updater = std::move(updater);
    }

}
