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
GEOMETRIX_STRONG_TYPEDEF(boost::units::quantity<boost::units::si::frequency>, GrowthRate);
BOOST_AUTO_TEST_CASE(StrongTypedefBoostUnits)
{
	using namespace boost::units;
	using time = quantity<si::time>;
	using type = decltype(GrowthRate() * time());
	static_assert(std::is_same<geometrix::tagged_quantity<GrowthRateTag, quantity<si::dimensionless>>, type>::value, "type should be dimensionless tagged_quantity");
}

#endif//! GEOMETRIX_TAGGED_QUANTITY_TESTS_HPP
