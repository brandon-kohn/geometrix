#include "derivative.hpp"

#include <geometrix/utility/scope_timer.ipp>

#include <boost/units/systems/si.hpp>

#include <gtest/gtest.h>

#include "google_test_fixture.hpp"

//////////////////////////////////////////////////////////////////////////
//!
//! Tests
//!

TEST(derivative_grammarTestSuite, testOptimizeAdd)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_expr<derivative_constant_terminal<0>> zero;
	derivative_expr<derivative_constant_terminal<1>> one;

	auto result0 = derivative_detail::optimize()(one * x + zero);
	static_assert(std::is_same<decltype(x), decltype(result0)>::value, "optimize not working");
}

TEST(derivative_grammarTestSuite, testOptimizeSubtract)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;	
	derivative_expr<derivative_constant_terminal<0>> zero;
	
	auto result0 = derivative_detail::optimize()(x - zero);
	static_assert(std::is_same<decltype(x), decltype(result0)>::value, "optimize not working");
}

TEST(derivative_grammarTestSuite, testOptimizeDivide)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_expr<derivative_constant_terminal<1>> one;
	derivative_detail::optimize opt;

	auto result0 = opt(x / one);
	static_assert(std::is_same<decltype(x), decltype(result0)>::value, "optimize not working");

	derivative_expr<derivative_constant_terminal<0>> zero;
	auto result1 = opt(zero / x);
	static_assert(std::is_same<decltype(zero), decltype(result1)>::value, "optimize not working");

	auto result2 = opt((x-x) / (x*x));
}

TEST(derivative_grammarTestSuite, testOptimizeProductDerivative)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_expr<derivative_constant_terminal<1>> one;
	derivative_detail::optimize opt;

	const derivative_expr<boost::proto::exprns_::expr<boost::proto::tagns_::tag::terminal, boost::proto::argsns_::term<x_var>, 0> > & a1 = x;
	const derivative_expr<boost::proto::exprns_::expr<boost::proto::tagns_::tag::terminal, boost::proto::argsns_::term<x_var>, 0> > & a2 = x;
	const derivative_expr<boost::proto::exprns_::expr<boost::proto::tagns_::tag::terminal, boost::proto::argsns_::term<derivative_constant<1> >, 0> > & da1 = one;
	const derivative_expr<boost::proto::exprns_::expr<boost::proto::tagns_::tag::terminal, boost::proto::argsns_::term<derivative_constant<1> >, 0> > & da2 = one;
	
	auto result0 = opt(da1 * a2 + a1 * da2);
	auto result1 = opt(opt(one * x) + opt(x * one));
	static_assert(std::is_same<decltype(result1), decltype(result0)>::value, "optimize not working");
}

TEST(derivative_grammarTestSuite, testOptimizeQuotient)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_expr<derivative_constant_terminal<1>> one;
	derivative_detail::optimize opt;

	const derivative_expr<boost::proto::exprns_::expr<boost::proto::tagns_::tag::terminal, boost::proto::argsns_::term<x_var>, 0> > & a1 = x;
	auto a2 = x * x;
	const derivative_expr<boost::proto::exprns_::expr<boost::proto::tagns_::tag::terminal, boost::proto::argsns_::term<derivative_constant<1> >, 0> > & da1 = one;
	auto da2 = x + x;
	auto result0 = opt((da1 * a2 - da2 * a1) / (a2 * a2));
	auto dresult = result0(2.0);
}

TEST(derivative_grammarTestSuite, testDerivativeGrammar)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	auto d0 = derivative(x);
	double result = d0(0);
	EXPECT_EQ(1.0, result);

	result = derivative(boost::proto::as_expr(3))(0);
	EXPECT_EQ(0.0, result);

	auto d = derivative(x + 3);
	result = d(5.0);
	EXPECT_EQ(1.0, result);

	auto d2 = derivative(x+x);
	result = d2(6.0);
	EXPECT_EQ(2.0, result);

 	auto d3 = derivative(x*x);
 	result = d3(6);
	EXPECT_EQ(12.0, result);

	auto d4 = derivative(x / (x*x));
	result = d4(2.0);
	EXPECT_EQ(-0.25, result);
}

TEST(derivative_grammarTestSuite, testConstantTimesDegree1Poly_ReturnsConstant)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	double constant = 4.0;
	auto d0 = derivative(constant * x + 1.0);
	double result = d0(0);

	EXPECT_EQ(constant, result);
}

TEST(derivative_grammarTestSuite, testConstantTimesDegree2Poly_ReturnsConstantTimes2TimesVariableEval)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	double constant = 4.0;
	auto d0 = derivative(constant * x * x + 1.0);
	double result = d0(2.0);

	EXPECT_EQ(2.0 * 2.0 * constant, result);
}

TEST(derivative_grammarTestSuite, testConstantTimesDegree3Poly_Returns68)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	double constant = 4.0;	
	auto d0 = derivative(constant * x * x * x + constant * x * x + constant * x + 1.0);

	//! Derivative should be c*3x^2 + c*2x + c = 12.0 * x^2 + 8.0 * x + 4.0 = 12.0 * 4.0 + 8.0 * 2.0 + 4.0 = 48.0 + 16.0 + 4.0 = 68.0
	double result = d0(2.0);

	EXPECT_EQ(68.0, result);
}

TEST(derivative_grammarTestSuite, testPowDerivative)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;
	pow_fun<2> fn;
	const derivative_expr<boost::proto::exprns_::expr<boost::proto::tagns_::tag::terminal, boost::proto::argsns_::term<x_var>, 0>> arg;
	derivative_expr<boost::proto::exprns_::expr<boost::proto::tagns_::tag::terminal, boost::proto::argsns_::term<derivative_constant<1>>, 0>> darg;

	auto d = chain_rule()(fn, arg, darg);
	auto d0 = derivative(pow<2>(x));

	double result = d0(2.0);

	EXPECT_EQ(4.0, result);
}

TEST(derivative_grammarTestSuite, testPowDerivative4)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	auto d0 = derivative(pow<4>(x));

	double result = d0(2.0);

	EXPECT_EQ(32.0, result);
}

TEST(derivative_grammarTestSuite, testPowDerivativePolynomialDegree2)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	//! = 2 * pow<1>(x*x + 2 * x) * 2x + 2
	//! = 2 * (x*x + 2*x) * 2x + 2 | x = 2
	//! = 2 * (4 + 4) * (4 + 2)
	//! = 2 * 8 * 6
	//! = 2 * 48
	//! = 96
	auto d0 = derivative(pow<2>(x*x + 2.0*x));

	double result = d0(2.0);

	EXPECT_EQ(96.0, result);
}

TEST(derivative_grammarTestSuite, testSinDerivative)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	auto d0 = derivative(sin(x));

	double result = d0(2.0);

	EXPECT_EQ(std::cos(2.), result);
}

TEST(derivative_grammarTestSuite, testCosDerivative)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	auto d0 = derivative(cos(x));

	double result = d0(2.0);

	EXPECT_EQ(-1.0 * std::sin(2.), result);
}

TEST(derivative_grammarTestSuite, testExpDerivative)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	auto d0 = derivative(exp(x));

	double result = d0(2.0);

	EXPECT_EQ(std::exp(2.), result);
}

TEST(derivative_grammarTestSuite, testLogDerivative)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	auto d0 = derivative(log(pow<3>(x)));

	double result = d0(4.0);

	EXPECT_EQ(0.75, result);
}

TEST(derivative_grammarTestSuite, testSqrtDerivative)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	auto d0 = derivative(sqrt(pow<3>(x)));

	double result = d0(4.0);

	EXPECT_EQ(3.0, result);
}

TEST(derivative_grammarTestSuite, testSqrtDerivativeWithOptimization)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_expr<boost::proto::terminal<derivative_constant<0>>::type> zero;
	derivative_grammar derivative;

	auto d0 = derivative(sqrt(pow<3>(x - zero)));
	double result = d0(4.0);

	EXPECT_EQ(3.0, result);
}

TEST(derivative_grammarTestSuite, testMulipleDerivative)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	//! 24 * x 
	//! = 24 * 4 
	//! = 96
	auto d0 = derivative(derivative(derivative(x*x*x*x)));

	auto result = d0(4.0);

	EXPECT_EQ(96.0, result);
}

TEST(derivative_grammarTestSuite, testUnitsSingleDerivative)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	auto d0 = derivative(x);

	auto result = d0(4.0 * boost::units::si::meters);

	EXPECT_TRUE(1.0 == result);
}

TEST(derivative_grammarTestSuite, testUnitsPolynomialDegree2Derivative)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	auto d0 = derivative(x*x);

	auto result = d0(4.0 * boost::units::si::meters);

	EXPECT_TRUE(8.0 * boost::units::si::meters == result);
}

TEST(derivative_grammarTestSuite, testUnitsPolynomialDegree3Derivative)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	auto d0 = derivative(x*x*x);

	auto result = d0(4.0 * boost::units::si::meters);

	EXPECT_TRUE(48.0 * boost::units::si::square_meters == result);
}

TEST(derivative_grammarTestSuite, testUnitsPolynomialDegree3Derivative2)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	auto d0 = derivative(derivative(x*x*x));

	auto result = d0(4.0 * boost::units::si::meters);

	EXPECT_TRUE(24.0 * boost::units::si::meters == result);
}

TEST(derivative_grammarTestSuite, testComplexDerivative)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;
		
	auto d0 = derivative( sqrt( pow<2>(x) * sin(x) ) );

	//! d0 == (x*x * cos(x) + 2 * x * sin(x)) / (2 * sqrt(x*x*sin(x)))
	auto result = d0(4.0);

	double v = 4.0;
	double expected = (v*v * std::cos(v) + 2.0 * v * std::sin(v)) / (2.0 * std::sqrt(std::abs(v*v*std::sin(v))));

	EXPECT_EQ(expected, result);
}

TEST(derivative_grammarTestSuite, testUnitsPolynomialDegree3Derivative2WithUnitConstant)
{
	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;

	auto d0 = derivative(derivative(x*x*x + 2.0 * boost::units::pow<3>(boost::units::si::meters)));

	auto result = d0(4.0 * boost::units::si::meters);

	EXPECT_TRUE(24.0 * boost::units::si::meters == result);
}

//! Timings...

//! Test the above mock registrations.
TEST(derivative_grammarTestSuite, time_grammar_evaluation)
{
	using namespace ::testing;

	derivative_expr<boost::proto::terminal<x_var>::type> x;
	derivative_grammar derivative;
#ifdef NDEBUG
	std::size_t nRuns = 100000000;
#else
	std::size_t nRuns = 100;
#endif
	std::vector<double> results(nRuns,0), results1(nRuns,0);

	{
		//auto d0(x + pow<2>(x) + pow<3>(x) + pow<4>(x) + pow<5>(x) + pow<6>(x) + pow<7>(x) + pow<8>(x) + pow<9>(x));		
		//auto d1(x + pow<2>(x) * (1.0 + x + pow<2>(x) + pow<3>(x) + pow<4>(x) + pow<5>(x) + pow<6>(x) + pow<7>(x)));
		//auto d2(x + pow<2>(x) * (1.0 + x + pow<2>(x) * (1.0 + x + pow<2>(x) + pow<3>(x) + pow<4>(x) + pow<5>(x))));
		//auto d3(x + pow<2>(x) * (1.0 + x + pow<2>(x) * (1.0 + x + pow<2>(x) * (1.0 + x + pow<2>(x) + pow<3>(x)))));
		auto d4 = derivative(x + pow<2>(x) * (1.0 + x + pow<2>(x) * (1.0 + x + pow<2>(x) * (1.0 + x + pow<2>(x) * (1.0 + x)))));

		auto d = derivative(x + pow<2>(x) + pow<3>(x) + pow<4>(x) + pow<5>(x) + pow<6>(x) + pow<7>(x) + pow<8>(x) + pow<9>(x));
		boost::proto::display_expr(d, std::cout);

		boost::proto::display_expr(d4, std::cout);
		GEOMETRIX_MEASURE_SCOPE_TIME("eval_grammar");
		for (int i = 0; i < nRuns; ++i)
			results[i] = d(7.7);
	}

	{
		GEOMETRIX_MEASURE_SCOPE_TIME("eval_byhand");
		double v = 7.7;
		for (int i = 0; i < nRuns; ++i)
			results1[i] = 1.0 + 2.0 * v + 3.0 * pow(v, 2) + 4.0 * pow(v, 3) + 5.0 * pow(v, 4) + 6.0 * pow(v, 5) + 7.0 * pow(v, 6) + 8.0 * pow(v, 7) + 9.0 * pow(v, 8);
	}
		
	{
		auto d = derivative(x + pow<2>(x) * (1.0 + x + pow<2>(x) * (1.0 + x + pow<2>(x) * (1.0 + x + pow<2>(x) * (1.0 + x)))));
		GEOMETRIX_MEASURE_SCOPE_TIME("eval_grammar2");
		for (int i = 0; i < nRuns; ++i)
			results[i] = d(7.7);
	}

	{
		GEOMETRIX_MEASURE_SCOPE_TIME("eval_byhand2");
		double v = 7.7;
		for (int i = 0; i < nRuns; ++i)
			results1[i] = 1.0 + 2.0 * v + 3.0 * pow(v, 2) + 4.0 * pow(v, 3) + 5.0 * pow(v, 4) + 6.0 * pow(v, 5) + 7.0 * pow(v, 6) + 8.0 * pow(v, 7) + 9.0 * pow(v, 8);
	}

	EXPECT_EQ(results, results1);
}

void StandardExceptionTranslator(const std::exception& e)
{
	BOOST_TEST_MESSAGE(e.what());
}

boost::unit_test::test_suite* init_unit_test_suite(int, char*[])
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_messages);
	boost::unit_test::unit_test_monitor.register_exception_translator<std::exception>(&StandardExceptionTranslator);
	boost::unit_test::framework::master_test_suite().p_name.value = "Geometrix Testing Framework";
	return 0;
}
