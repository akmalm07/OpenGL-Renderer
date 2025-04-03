#pragma once


#include "config.h"

#include <glm/glm.hpp>


#define INT(x) static_cast<int>(x)
#define SIZET(x) static_cast<size_t>(x)
#define UINT(x) static_cast<unsigned int>(x)


#define UINT32(x) static_cast<uint32_t>(x)  

#define ASSERT(cond, msg) if (!(cond)) { std::cerr << msg << std::endl; } 

#define MOVE_ITR(x) std::make_move_iterator(x)

#define BOOL(x) static_cast<bool>(x)

#define PRINT_VEC3(str, v) std::cout << str << ": " << v.x << " " << v.y << " " << v.z << std::endl;

#define PRINT_MAT4(str, v) std::cout << str << ": \n" << v[0][0] << " " << v[0][1] << " " << v[0][2] << " " << v[0][3] << std::endl; \
std::cout << v[1][0] << " " << v[1][1] << " " << v[1][2] << " " << v[1][3] << std::endl; \
std::cout << v[2][0] << " " << v[2][1] << " " << v[2][2] << " " << v[2][3] << std::endl; \
std::cout << v[3][0] << " " << v[3][1] << " " << v[3][2] << " " << v[3][3] << std::endl;

#define UINT(x) static_cast<unsigned int>(x)

#define INT(x) static_cast<int>(x)

#define KEY_MAX 1024

namespace glType
{
	using Index = unsigned int;
	using Vertex = float;


	template <typename T, size_t S = 0>
	struct ArrOrVec
	{
		static constexpr bool value = false;
	};

	template <typename T>
	struct ArrOrVec<std::vector<T>, 0>
	{
		static constexpr bool value = true;
	};

	template <typename T, size_t S>
	struct ArrOrVec<std::array<T, S>, S>
	{
		static constexpr bool value = true;
	};

	template<class T, size_t S = 0>
	concept IsArrOrVec = ArrOrVec<T, S>::value;


	template <class T>
	concept IsClass = std::is_class_v<T>;


	template <class T>
	concept Callible = std::is_invocable_v<T>;



	//template<class Func, class ... Args>
	//concept BoolLambdaVardic = std::invocable<Func, Args...>&& std::convertible_to<std::invoke_result_t<Func, Args...>, bool>;


	template <typename T>
	concept Enum = std::is_enum_v<T>;


	template <class T>
	concept GLM_TYPE = std::same_as<T, glm::vec2> || std::same_as<T, glm::vec3> ||
		std::same_as<T, glm::vec4>;

	template <class T>
	concept GLM_VEC_TYPE = GLM_TYPE<T> && requires(T t) { typename T::value_type; };

	template <class T>
	concept GLMVec = std::same_as<T, glm::vec2> || std::same_as<T, glm::vec3> ||
		std::same_as<T, glm::vec4> || std::same_as<T, std::nullptr_t>;

	template<class T>
	concept IndexType = std::is_same_v<T, uint32_t> || std::is_same_v<T, uint16_t>;


	template<class T>
	concept Arithmatic = std::is_arithmetic_v<T>;


	//template <GLMVec POS, GLMVec COL, GLMVec NORM, GLMVec TEX>
	//struct Vertex
	//{
	//	using Position = POS;
	//	using Color = COL;
	//	using Normal = NORM;
	//	using Texture = TEX;
	//};

}