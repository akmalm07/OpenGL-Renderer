#pragma once

#include "tools\include\keys.h"

#include <mutex>


namespace tools
{
    struct CallbackInput { virtual ~CallbackInput() = default; };

    struct KeyCombInputOne : CallbackInput
    {
        Keys number;
        Action action;
        Mods mod = Mods::None;

        KeyCombInputOne(Keys key, Action action)
            : number(key), action(action) {
        }

        KeyCombInputOne(Keys key, Action action, Mods mod)
            : number(key), action(action), mod(mod) {
        }
    };

    struct KeyCombInputPoly : CallbackInput
    {
        std::array<Keys, KEY_MAX> number;
        Action action;
        Mods mod = Mods::None;

        KeyCombInputPoly(std::array<Keys, KEY_MAX> key, Action action)
            : number(key), action(action) {
        }
    };

    struct MouseButtonInput : CallbackInput
    {
        Mouse button;
        Action action;
		MouseButtonInput(Mouse button, Action action)
			: button(button), action(action) {
		}
    };

    struct AABButtonInput : CallbackInput
    {
        float cordX, cordY, width, height;
        Action action;
        Mouse button;
        std::string_view name;

		AABButtonInput(float x, float y, float w, float h, Action action, Mouse button, std::string_view name)
			: cordX(x), cordY(y), width(w), height(h), action(action), button(button), name(name) {
		}
    };

    struct MouseMoveInput : CallbackInput
    {
        MouseChange change;
        Mouse button = Mouse::None;

		MouseMoveInput(MouseChange change, Mouse button = Mouse::None)
			: change(change), button(button) {
		}
    };


	template<typename T>
    concept CallbackInputConcept = std::is_base_of_v<CallbackInput, T>;


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