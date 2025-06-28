#pragma once



template<typename T>
constexpr size_t SIZET(T x) {
	return static_cast<size_t>(x);

}

constexpr size_t MAX_NUM_OF_ENTITIES = 5000;

template<typename T>
constexpr unsigned int UINT32(T x) {
	return static_cast<uint32_t>(x);
}
//#define MOVE_ITR(x) std::make_move_iterator(x)

template<typename T>
constexpr unsigned int BOOL(T x) {
	return static_cast<unsigned int>(x);
}
#define PRINT_VEC3(str, v) std::cout << str << ": " << v.x << " " << v.y << " " << v.z << std::endl;

#define PRINT_MAT4(str, v) std::cout << str << ": \n" << v[0][0] << " " << v[0][1] << " " << v[0][2] << " " << v[0][3] << std::endl; \
std::cout << v[1][0] << " " << v[1][1] << " " << v[1][2] << " " << v[1][3] << std::endl; \
std::cout << v[2][0] << " " << v[2][1] << " " << v[2][2] << " " << v[2][3] << std::endl; \
std::cout << v[3][0] << " " << v[3][1] << " " << v[3][2] << " " << v[3][3] << std::endl;

template<typename T>
constexpr unsigned int UINT(T x) {
	return static_cast<unsigned int>(x);
}

template<typename T>
constexpr int INT(T x) {
	return static_cast<int>(x);
}

template<typename T>
constexpr float FLOAT(T x) {
	return static_cast<float>(x);
}



#define KEY_MAX 1024

namespace glType
{

	using Entity = uint64_t;
	


	template<typename T>
	class Component;

	template<class T>
	concept ComponentType = std::is_base_of_v<Component<T>, T>;



	template<glType::ComponentType T>
	class ComponentRegistry;


	class ComponentRegistryBase
	{
	public:

		/*
		* 
		template<glType::ComponentType T>
		std::unordered_map<glType::Entity, T>& get_entities() const// Fix This so that you can access all the desired
		{
			return static_cast<ComponentRegistry<T>*>(this)->_components;
		}
		*
		*/


	};

	template<typename U>
	concept HasCommunicateImpl =
		std::is_base_of_v<Component<U>, U>&&
		requires(const U& t) {
			{ t.communicate_impl(std::declval<Entity>()) } -> std::same_as<void>;
	};

	
	template<typename U>
	concept HasCheckImpl =
		std::is_base_of_v<Component<U>, U>&&
		requires(const U& t) {
			{ t.check(std::declval<const std::unordered_map<Entity, U>&>()) } -> std::same_as<void>;
	};


	template<typename T>
	class Component
	{
	public:

		void check(const std::unordered_map<Entity, T>& components) requires HasCheckImpl<T>
		{
			static_cast<T*>(this)->check_impl(components);
		}
		
		void communicate(Entity entity) // requires HasCommunicateImpl<T>//ERR!
		{
			static_cast<T*>(this)->communicate_impl(entity);
		}



		virtual ~Component() = default;
	};


	using Index = unsigned int;
	using Vertex = float;

	enum class BoundType
	{
		AABB = 0,
		OBB,
		Sphere
	};


	enum class MovementType
	{
		Static,
		Constant,
		Kenimatic
	};


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