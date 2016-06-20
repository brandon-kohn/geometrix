//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TAGGED_QUANTITY_TESTS_HPP
#define GEOMETRIX_TAGGED_QUANTITY_TESTS_HPP

#include <boost/test/included/unit_test.hpp>

#include <type_traits>
#include <utility>

#include <geometrix/utility/tagged_quantity.hpp>
#include <geometrix/arithmetic/scalar_arithmetic.hpp>
#include <geometrix/numeric/boost_units_quantity.hpp>
#include <geometrix/arithmetic/boost_units_arithmetic.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>

namespace
{
	GEOMETRIX_STRONG_TYPEDEF(double, Dummy);
	inline void dummy(Dummy) {}
}

BOOST_AUTO_TEST_CASE(ConstructTaggedQuantity_DefaultCtor_CompilesNoThrow)
{
	auto sut = Dummy{};
	static_assert(std::is_same<decltype(sut), Dummy>::value, "sut should be Dummy");
}

BOOST_AUTO_TEST_CASE(ConstructTaggedQuantity_ValueCtor_ValueOfSUTEqualsValueUseInCtor)
{
	auto sut = Dummy{1.0};
	BOOST_CHECK_EQUAL(1.0, sut);
}

BOOST_AUTO_TEST_CASE(AddTaggedQuantity_AddOneToZero_HasValueOne)
{
	auto sut = Dummy{ 0.0 };
	sut = sut + 1.0;
	BOOST_CHECK_EQUAL(1.0, sut);
}

BOOST_AUTO_TEST_CASE(SubtractTaggedQuantity_SubtractTenFromOne_HasValueMinus9)
{
	auto sut = Dummy{ 1.0 };
	sut = sut - 10.0;
	BOOST_CHECK_EQUAL(-9.0, sut);
}

BOOST_AUTO_TEST_CASE(MultiplyTaggedQuantity_MultiplyOneTo10_HasValue10)
{
	auto sut = Dummy{ 1.0 };
	sut = sut * 10.0;
	BOOST_CHECK_EQUAL(10.0, sut);
}

BOOST_AUTO_TEST_CASE(DivideTaggedQuantity_DivideOneBy10_HasValueOne_10th)
{
	auto sut = Dummy{ 1.0 };
	sut = sut / 10.0;
	BOOST_CHECK_EQUAL(0.1, sut);
}

BOOST_AUTO_TEST_CASE(CallFunctionWithTaggedQuantity_ShouldCompile)
{
	auto value = double{ 10.0 };
	auto d = Dummy{ value };

	//dummy(value);//!should not compile.
	dummy(d);
}

BOOST_AUTO_TEST_CASE(SwapTest_TaggedTypeWithTaggedType_ValuesSwapped)
{
	auto d1 = Dummy{ 10.0 };
	auto d2 = Dummy{ 20.0 };
	std::swap(d1, d2);

	BOOST_CHECK_EQUAL(20.0, d1);
	BOOST_CHECK_EQUAL(10.0, d2);
}

BOOST_AUTO_TEST_CASE(SwapTest_TaggedTypeWithRawType_ValuesSwapped)
{
	auto d1 = Dummy{ 10.0 };
	auto d2 = double{ 20.0 };
	d1.swap(d2);

	BOOST_CHECK_EQUAL(20.0, d1);
	BOOST_CHECK_EQUAL(10.0, d2);
}

BOOST_AUTO_TEST_CASE(BoolConversion_TaggedTypeWithTrueValue_ReturnsTrue)
{
	auto v = Dummy{ 1.0 };
	BOOST_CHECK(v);
}

BOOST_AUTO_TEST_CASE(BoolConversion_TaggedTypeWithBoolValueTypeAndTrueValue_ReturnsTrue)
{
	GEOMETRIX_STRONG_TYPEDEF(bool, ReallyRealBool);
	
	auto v = ReallyRealBool{ true };	

	BOOST_CHECK_EQUAL(true, v);
}

BOOST_AUTO_TEST_CASE(ImplicitTypeConversion_TaggedTypeWithDoubleValueType_ImplicitlyConvertsToUnderlyingType)
{
	auto v = double{ 10.0 };
	auto d = Dummy{ v };
	BOOST_CHECK_EQUAL(v, d);
}

BOOST_AUTO_TEST_CASE(LessThanOperation_TaggedQuantityVsUnderlyingTypeWhichIsLess_ReturnsTrue)
{
	auto d = Dummy{ 10.0 };
	double v = 5.0;

	auto result = v < d;

	BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(LessThanOperation_UnderlyingTypeVsTaggedQuantityWhichIsLess_ReturnsTrue)
{
	auto d = Dummy{ 5.0 };
	double v = 10.0;

	auto result = d < v;

	BOOST_CHECK(result);
}

#include <ostream>
namespace geometrix {
	template <typename Tag, typename Number>
	inline std::ostream& operator << (std::ostream& os, const geometrix::tagged_quantity<Tag, Number>& q)
	{
		os << q.value();
		return os;
	}
}

namespace
{
	GEOMETRIX_STRONG_TYPEDEF(double, ValueA);
	GEOMETRIX_STRONG_TYPEDEF(double, ValueB);

	inline std::tuple<ValueA, ValueB> make_values(double a, double b)
	{
		return std::make_tuple(ValueA(a), ValueB(b));
	}
}


BOOST_AUTO_TEST_CASE(StrongTypedefStdTie_AssignsToRawType)
{
	auto a = double{ 'a' };
	auto b = double{ 'b' };

	ValueA ta;
	ValueB tb;
	std::tie(ta, tb) = make_values(a, b);

	BOOST_CHECK_EQUAL(a, ta);
	BOOST_CHECK_EQUAL(b, tb);
}

#include <boost/units/systems/si.hpp>


#include <boost/units/quantity.hpp>
#include <boost/units/pow.hpp>
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/prefixes.hpp>
#include <boost/units/make_scaled_unit.hpp>

namespace probability_system {
	using namespace boost::units;

	struct probability_base_dimension : base_dimension<probability_base_dimension, 1> {};
	using probability_dimension = probability_base_dimension::dimension_type;

	struct probability_base_unit : base_unit<probability_base_unit, probability_dimension, 1>
	{};

	using system = make_system<probability_base_unit>::type;
	using probability_unit = unit<probability_dimension, system>;

	BOOST_UNITS_STATIC_CONSTANT(proportion, probability_unit);
	BOOST_UNITS_STATIC_CONSTANT(proportions, probability_unit);
	BOOST_UNITS_STATIC_CONSTANT(fraction, probability_unit);
	BOOST_UNITS_STATIC_CONSTANT(fractions, probability_unit);
	BOOST_UNITS_STATIC_CONSTANT(parts_per_unit, probability_unit);

	//! Percent
	using percent_base_unit = scaled_base_unit<probability_base_unit, scale<10, static_rational<-2>>>;
	using percent_system = make_system<percent_base_unit>::type;
	using percent_unit = unit<probability_dimension, percent_system>;

	BOOST_UNITS_STATIC_CONSTANT(percent, percent_unit);
}//! namespace percent_units;

namespace boost {
	namespace units {
		template<> struct base_unit_info<probability_system::probability_base_unit>
		{
			static std::string name() { return "probability"; }
			static std::string symbol() { return "P"; }
		};

		template<> struct base_unit_info<probability_system::percent_base_unit>
		{
			static std::string name() { return "percent"; }
			static std::string symbol() { return "%"; }
		};
		
		template<typename System1, typename X, typename System2, typename Y>
		inline quantity<probability_system::probability_unit, decltype(std::declval<X>() + std::declval<Y>())>
			operator+(const quantity<unit<probability_system::probability_dimension, System1>, X>& lhs,
				      const quantity<unit<probability_system::probability_dimension, System2>, Y>& rhs)
		{
			using type = quantity<probability_system::probability_unit, decltype(std::declval<X>() + std::declval<Y>())>;

			return type::from_value(lhs.value() + rhs.value());
		}
	}
}
#if BOOST_UNITS_HAS_BOOST_TYPEOF

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TYPE(probability_system::probability_base_unit)
BOOST_TYPEOF_REGISTER_TYPE(probability_system::probability_base_dimension)

#endif

using probability = boost::units::quantity<probability_system::probability_unit>;

GEOMETRIX_STRONG_TYPEDEF(boost::units::quantity<boost::units::si::wavenumber>, GrowthRate);
GEOMETRIX_STRONG_TYPEDEF(probability, LowerAsymptote);
GEOMETRIX_STRONG_TYPEDEF(probability, UpperAsymptote);
GEOMETRIX_STRONG_TYPEDEF(boost::units::quantity<boost::units::si::dimensionless>, GrowthSkew);
GEOMETRIX_STRONG_TYPEDEF(boost::units::quantity<boost::units::si::dimensionless>, InterceptCoef);
BOOST_AUTO_TEST_CASE(StrongTypedefBoostUnits)
{
	using namespace boost::units;
	using namespace geometrix;
	using std::pow;
	using std::exp;

	LowerAsymptote A(0.0 * probability_system::proportion);
	UpperAsymptote K(10.0 * probability_system::proportion);
	InterceptCoef Q(1.0);
	GrowthRate B(1.0 * si::reciprocal_meters);
	quantity<si::length> x = 1.0 * si::meters;
	GrowthSkew v(1.0);
	auto result = A + (K - A) / pow(1.0 + Q * exp(-B*x), v);
 	auto cmp = absolute_tolerance_comparison_policy<probability>(1e-10 * probability_system::proportion);
 	BOOST_CHECK(cmp.equals(7.3105857863000487 * probability_system::proportion, result.value()));
}

#include <geometrix/numeric/constants.hpp>
BOOST_AUTO_TEST_CASE(CosineStrongTypedef_DoubleQuantity_ReturnsTaggedQuantity)
{
	using namespace geometrix;

	Dummy val(geometrix::constants::pi<double>());

	auto result = cos(val);

	auto cmp = absolute_tolerance_comparison_policy<double>(1e-10);
	BOOST_CHECK(cmp.equals(-1.0, result.value()));
}

BOOST_AUTO_TEST_CASE(SineStrongTypedef_DoubleQuantity_ReturnsTaggedQuantity)
{
	using namespace geometrix;

	Dummy val(geometrix::constants::pi<double>());

	auto result = sin(val);

	auto cmp = absolute_tolerance_comparison_policy<double>(1e-10);
	BOOST_CHECK(cmp.equals(0.0, result.value()));
}

BOOST_AUTO_TEST_CASE(Atan2StrongTypedef_DoubleQuantity_ReturnsTaggedQuantity)
{
	using namespace geometrix;

	Dummy x(0.0);
	Dummy y(1.0);

	auto result = atan2(y, x);

	auto cmp = absolute_tolerance_comparison_policy<double>(1e-10);
	BOOST_CHECK(cmp.equals(geometrix::constants::half_pi<double>(), result.value()));
}

BOOST_AUTO_TEST_CASE(PowStrongTypedef_DoubleQuantity_ReturnsTaggedQuantity)
{
	using namespace geometrix;

	Dummy x(2.0);

	auto result = pow(x, x);

	auto cmp = absolute_tolerance_comparison_policy<double>(1e-10);
	BOOST_CHECK(cmp.equals(4.0, result.value()));
}

BOOST_AUTO_TEST_CASE(PowStrongTypedef_TaggedDoubleBaseRawDoubleExponent_ReturnsTaggedQuantity)
{
	using namespace geometrix;

	Dummy x(3.0);

	auto result = pow(x, 3.0);

	auto cmp = absolute_tolerance_comparison_policy<double>(1e-10);
	BOOST_CHECK(cmp.equals(27.0, result.value()));
}

BOOST_AUTO_TEST_CASE(PowStrongTypedef_RawDoubleBaseTaggedDoubleExponent_ReturnsTaggedQuantity)
{
	using namespace geometrix;

	Dummy x(3.0);

	auto result = pow(3.0, x);

	auto cmp = absolute_tolerance_comparison_policy<double>(1e-10);
	BOOST_CHECK(cmp.equals(27.0, result.value()));
}

#endif//! GEOMETRIX_TAGGED_QUANTITY_TESTS_HPP
