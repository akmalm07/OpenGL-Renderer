#include "headers.h"
#include "physics/include/units.h"

namespace units
{



	Measurible operator""_meters(long double val)
	{
		return val * UnitScale<Units::Meters>::value();
	}

	Measurible operator""_meter2(long double val)
	{
		return val * UnitScale<Units::Meters>::value() * UnitScale<Units::Meters>::value();
	}

	Measurible operator""_kilograms(long double val)
	{
		return val * UnitScale<Units::Kilograms>::value();
	}

	Measurible operator""_sec(long double val)
	{
		return val * UnitScale<Units::Seconds>::value();
	}

	Measurible operator""_newtons(long double val)
	{
		return val * UnitScale<Units::Kilograms>::value() * (UnitScale<Units::Meters>::value() / (UnitScale<Units::Seconds>::value() * UnitScale<Units::Seconds>::value()));
	}

	Measurible operator""_meters_per_sec(long double val)
	{
		return (val * UnitScale<Units::Meters>::value()) / UnitScale<Units::Seconds>::value();
	}

	Measurible operator""_meters_per_sec2(long double val)
	{
		return (val * UnitScale<Units::Meters>::value()) / (UnitScale<Units::Seconds>::value() * UnitScale<Units::Seconds>::value());
	}


	Measurible operator""_kg_per_meter3(long double val)
	{
		return (val * UnitScale<Units::Kilograms>::value()) / (UnitScale<Units::Meters>::value() * UnitScale<Units::Meters>::value() * UnitScale<Units::Meters>::value());
	}

	Measurible operator""_kg_per_meter2(long double val)
	{
		return (val * UnitScale<Units::Kilograms>::value()) / (UnitScale<Units::Meters>::value() * UnitScale<Units::Meters>::value());
	}

	Measurible operator""_kg_meters_per_sec(long double val)
	{
		return (val * UnitScale<Units::Meters>::value() * UnitScale<Units::Kilograms>::value()) / UnitScale<Units::Seconds>::value();
	}

	Measurible operator""_joules(long double val)
	{
		return val * UnitScale<Units::Kilograms>::value() * UnitScale<Units::Meters>::value() * UnitScale<Units::Meters>::value() / (UnitScale<Units::Seconds>::value() * UnitScale<Units::Seconds>::value());
	}

	Measurible operator""_watts(long double val)
	{
		return val * UnitScale<Units::Kilograms>::value() * UnitScale<Units::Meters>::value() * UnitScale<Units::Meters>::value() / (UnitScale<Units::Seconds>::value() * UnitScale<Units::Seconds>::value() * UnitScale<Units::Seconds>::value());
	}

	Measurible operator""_mu(long double val)
	{
		return val * UnitScale<Units::Meters>::value() / UnitScale<Units::Kilograms>::value();
	}

	Measurible operator""_to_deg(long double val)
	{
		return (val * UnitScale<Units::Radians>::value()) * (180.0L / 3.14159265358979323846L);
	}

	Measurible operator""_to_rad(long double val)
	{
		return (val * UnitScale<Units::Degrees>::value()) * (3.14159265358979323846L / 180.0L);
	}

	Measurible operator""_degrees(long double val)
	{
		return (val * UnitScale<Units::Degrees>::value());
	}

	Measurible operator""_radians(long double val)
	{
		return (val * UnitScale<Units::Radians>::value());
	}



	Measurible operator""_kg_meters_per_sec2(long double val)
	{
		return (val * UnitScale<Units::Meters>::value() * UnitScale<Units::Kilograms>::value()) / (UnitScale<Units::Seconds>::value() * UnitScale<Units::Seconds>::value());
	}

	Measurible operator""_kg_per_meter2_per_sec2(long double val)
	{
		return (val * UnitScale<Units::Kilograms>::value()) / (UnitScale<Units::Meters>::value() * UnitScale<Units::Seconds>::value() * UnitScale<Units::Seconds>::value());
	}

	Measurible operator""_kg_per_meter2_per_sec(long double val)
	{
		return (val * UnitScale<Units::Kilograms>::value()) / (UnitScale<Units::Meters>::value() * UnitScale<Units::Seconds>::value());
	}

	Measurible operator""_kg_per_meter3_per_sec2(long double val)
	{
		return (val * UnitScale<Units::Kilograms>::value()) / (UnitScale<Units::Meters>::value() * UnitScale<Units::Meters>::value() * UnitScale<Units::Seconds>::value() * UnitScale<Units::Seconds>::value());
	}

	Measurible operator""_kg_per_meter3_per_sec(long double val)
	{
		return (val * UnitScale<Units::Kilograms>::value()) / (UnitScale<Units::Meters>::value() * UnitScale<Units::Meters>::value() * UnitScale<Units::Seconds>::value());
	}


}