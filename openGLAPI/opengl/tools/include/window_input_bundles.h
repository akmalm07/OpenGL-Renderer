#pragma once

#include "tools\include\keys.h"

#include <mutex>


namespace tools
{
    struct AABButtonB;
    struct KeyCombB;
    struct MouseButtonB;

    struct KeyCombInputOne
    {
        KeyCombInputOne(Keys key, Action action)
            : number(key), action(action), mod(Mods::None) {
        }

        KeyCombInputOne(Keys key, Action action, Mods mod)
            : number(key), action(action), mod(mod) {
        }

        Keys number;
        Action action;
        Mods mod = Mods::None;
    };

    struct KeyCombInputPoly
    {
        KeyCombInputPoly(std::array<Keys, KEY_MAX> key, Action action)
            : number(key), action(action), mod(Mods::None) {
        }
        std::array<Keys, KEY_MAX> number;
        Action action;
        Mods mod = Mods::None;
    };

    struct MouseButtonInput
    {
        Mouse name;
        Action action;
    };

    struct AABButtonInput
    {
        float cordX, cordY, width, height;
        Action action;
        Mouse button;
        std::string_view name;
    };

    struct MouseMoveInput
    {
        MouseChange change;
        Mouse button = Mouse::None;
    };
}
    //struct ThreadControlInfo
    //{
    //public:
    //    ThreadControlInfo(const std::shared_ptr<ConditionalVariuble>& windowInput,
    //        const std::shared_ptr<ConditionalVariuble>& changingParams, const std::shared_ptr<std::mutex>& mtx);

    //    std::shared_ptr<ConditionalVariuble> windowInputWait;
    //    std::shared_ptr<ConditionalVariuble> changingParamsWait;
    //    std::shared_ptr<std::mutex> lock;
    //};