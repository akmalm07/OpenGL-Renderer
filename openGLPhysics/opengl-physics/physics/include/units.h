#pragma once

#include "config.h"


namespace units
{

	using Measurible = long double;

	template <typename T>
	inline Measurible MEASURIBLE(T val)
	{
		return static_cast<Measurible>(val);
	}

	enum class Units
	{
		Meters,
		Kilograms,
		Seconds,
		Degrees,
		Radians,

	};

	template <Units T>
	class UnitScale
	{
	public:
		static inline void set_scale(long double value)
		{
			_value = value;
		}

		static inline long double value()
		{
			return _value;
		}

	private:
		explicit UnitScale(long double value) : _value(value) {}

		inline static long double _value = 1.0;
	};

	template <>
	class UnitScale<Units::Degrees>
	{
	public:

		static inline long double value()
		{
			return _value;
		}

	private:
		inline static long double _value = 1.0L;
	};

	template <>
	class UnitScale<Units::Radians>
	{
	public:
		static inline long double value()
		{
			return _value;
		}
	private:
		inline static long double _value = 1.0L;
	};


	Measurible operator"" _meters(long double val);

	Measurible operator"" _meter2(long double val);

	Measurible operator"" _kilograms(long double val);

	Measurible operator"" _sec(long double val);

	Measurible operator"" _newtons(long double val);

	Measurible operator"" _meters_per_sec(long double val);

	Measurible operator"" _meters_per_sec2(long double val);

	Measurible operator"" _kg_per_meter3(long double val);
	
	Measurible operator"" _kg_per_meter2(long double val);

	Measurible operator"" _kg_meters_per_sec(long double val);

	Measurible operator""_kg_meters_per_sec2(long double val);

	Measurible operator""_kg_per_meter2_per_sec2(long double val);

	Measurible operator""_kg_per_meter2_per_sec(long double val);

	Measurible operator""_kg_per_meter3_per_sec2(long double val);

	Measurible operator""_kg_per_meter3_per_sec(long double val);

	Measurible operator"" _joules(long double val);

	Measurible operator"" _watts(long double val);

	Measurible operator"" _mu(long double val);
	
	Measurible operator"" _to_deg(long double val); // theta;
	
	Measurible operator"" _to_rad(long double val); // phi
	

	Measurible operator"" _degrees(long double val);

	Measurible operator"" _radians(long double val);


	template <glm::length_t L, typename T, glm::qualifier Q>
	glm::vec<L, T, Q> to_meters_per_sec(const glm::vec<L, T, Q>& vec)
	{
		auto scale = UnitScale<Units::Meters>::value() / UnitScale<Units::Seconds>::value();
		return vec * static_cast<T>(scale);
	}

	template <glm::length_t L, typename T, glm::qualifier Q>
	glm::vec<L, T, Q> to_meters_per_sec2(const glm::vec<L, T, Q>& vec)
	{
		auto scale = UnitScale<Units::Meters>::value() / (UnitScale<Units::Seconds>::value() * UnitScale<Units::Seconds>::value());
		return vec * static_cast<T>(scale);
	}

	template <glm::length_t L, typename T, glm::qualifier Q>
	glm::vec<L, T, Q> to_meters(const glm::vec<L, T, Q>& vec)
	{
		auto scale = UnitScale<Units::Meters>::value();
		return vec * static_cast<T>(scale);
	}


	template <glm::length_t L, typename T, glm::qualifier Q>
	glm::vec<L, T, Q> to_newtons(const glm::vec<L, T, Q>& vec)
	{
		auto scale = UnitScale<Units::Kilograms>::value() * UnitScale<Units::Meters>::value() / (UnitScale<Units::Seconds>::value() * UnitScale<Units::Seconds>::value());
		return vec * static_cast<T>(scale);
	}




};

