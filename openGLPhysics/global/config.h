#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <string_view>
#include <optional>
#include <stdint.h>
#include <functional>
#include <filesystem>
#include <cmath>
#include <numbers>

#include <glm/glm.hpp>

#include "tools/include/types.h"

#include <GLFW/glfw3.h>

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

namespace tools
{
	template<typename T>
	using view_ptr = const T* const;

	template<typename T>
	using view_ptr_non_const = T const*;

	template<typename T>
	using view_ptr_mutable = T* const;

}