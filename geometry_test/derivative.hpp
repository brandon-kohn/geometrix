#include <boost/proto/proto.hpp>
#include <boost/proto/context/default.hpp>
#include <boost/mpl/int.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/cmath.hpp>
#include <type_traits>

struct x_var {};

template <int v>
struct derivative_constant
{
	using result_type = int;
	static const int value = v;

	template <typename ...Args>
	result_type operator()(const Args&... a) const { return v; }

	operator result_type() const { return v; }
};

template <int V>
using derivative_constant_terminal = typename boost::proto::terminal<derivative_constant<V>>::type;

struct derivative_grammar;

template<typename Expr>
struct derivative_expr;

namespace derivative_detail
{
	struct optimize
		: boost::proto::switch_<struct optimize_cases_>
	{};

	struct expr_generator
	{
		BOOST_PROTO_CALLABLE()
		BOOST_PROTO_USE_BASIC_EXPR()

		template<typename Sig>
		struct result;

		template<typename This, typename Expr>
		struct result<This(Expr)>
		{
			typedef derivative_expr<Expr> type;
		};

		template<typename This, typename Expr>
		struct result<This(Expr &)>
		{
			typedef derivative_expr<Expr> type;
		};

		template<typename This, typename Expr>
		struct result<This(Expr const &)>
		{
			typedef derivative_expr<Expr> type;
		};

		template<typename Expr>
		derivative_expr<Expr> operator ()(Expr const &e) const
		{
			derivative_expr<Expr> that = { e };
			return that;
		}
	};

}

struct domain
	: boost::proto::domain< derivative_detail::expr_generator, derivative_grammar >
{
	template< typename T >
	struct as_child
		: proto_base_domain::as_expr< T >
	{};
};

template< int Exp >
struct pow_fun
{
	template <typename Sig>
	struct result;

	template <typename This, typename T>
	struct result<This(T)>
	{
		using type = T;
		BOOST_FORCEINLINE static type apply(const T& t) { using std::pow; return pow(t, Exp); }
	};

	template <typename This, typename Units, typename T>
	struct result<This(boost::units::quantity<Units,T>)>
	{
		using type = decltype(boost::units::pow<Exp>(std::declval<boost::units::quantity<Units,T>>()));
		BOOST_FORCEINLINE static type apply(const boost::units::quantity<Units,T>& t) { using boost::units::pow; return pow<Exp>(t); }
	};

	template <typename T>
	BOOST_FORCEINLINE typename result<pow_fun<Exp>(T)>::type operator()(const T& d) const
	{
		return result<pow_fun<Exp>(T)>::apply(d);
	}
};

template< int Exp, typename Arg >
BOOST_FORCEINLINE typename boost::proto::result_of::make_expr<boost::proto::tag::function, pow_fun< Exp >, Arg>::type const pow(Arg arg)
{
	return boost::proto::make_expr<boost::proto::tag::function>(pow_fun<Exp>(), arg);
}

struct sin_fun
{
	template <typename Sig>
	struct result;

	template <typename This, typename T>
	struct result<This(T)>
	{
		using type = T;

		BOOST_FORCEINLINE static type apply(const T& t) { using std::sin; return sin(t); }
	};

	template <typename This, typename Units, typename T>
	struct result<This(boost::units::quantity<Units, T>)>
	{
		using type = decltype(boost::units::sin(std::declval<boost::units::quantity<Units, T>>()));
		BOOST_FORCEINLINE static type apply(const boost::units::quantity<Units, T>& t) { using boost::units::sin; return sin(t); }
	};

	template <typename T>
	BOOST_FORCEINLINE typename result<sin_fun(T)>::type operator()(const T& d) const
	{
		return result<sin_fun(T)>::apply(d);
	}
};

template< typename Arg >
BOOST_FORCEINLINE typename boost::proto::result_of::make_expr<boost::proto::tag::function, sin_fun, Arg>::type const sin(Arg arg)
{
	return boost::proto::make_expr<boost::proto::tag::function>(sin_fun(), arg);
}

struct cos_fun
{
	template <typename Sig>
	struct result;

	template <typename This, typename T>
	struct result<This(T)>
	{
		using type = T;

		BOOST_FORCEINLINE static type apply(const T& t) { using std::cos; return cos(t); }
	};

	template <typename This, typename Units, typename T>
	struct result<This(boost::units::quantity<Units, T>)>
	{
		using type = decltype(boost::units::cos(std::declval<boost::units::quantity<Units, T>>()));
		BOOST_FORCEINLINE static type apply(const boost::units::quantity<Units, T>& t) { using boost::units::cos; return cos(t); }
	};

	template <typename T>
	BOOST_FORCEINLINE typename result<cos_fun(T)>::type operator()(const T& d) const
	{
		return result<cos_fun(T)>::apply(d);
	}
};

template< typename Arg >
BOOST_FORCEINLINE typename boost::proto::result_of::make_expr<boost::proto::tag::function, cos_fun, Arg>::type const cos(Arg arg)
{
	return boost::proto::make_expr<boost::proto::tag::function>(cos_fun(), arg);
}

struct exp_fun
{
	template <typename Sig>
	struct result;

	template <typename This, typename T>
	struct result<This(T)>
	{
		using type = T;

		BOOST_FORCEINLINE static type apply(const T& t) { using std::exp; return exp(t); }
	};

	template <typename This, typename Units, typename T>
	struct result<This(boost::units::quantity<Units, T>)>
	{
		using type = decltype(boost::units::exp(std::declval<boost::units::quantity<Units, T>>()));
		BOOST_FORCEINLINE static type apply(const boost::units::quantity<Units, T>& t) { using boost::units::exp; return exp(t); }
	};

	template <typename T>
	BOOST_FORCEINLINE typename result<exp_fun(T)>::type operator()(const T& d) const
	{
		return result<exp_fun(T)>::apply(d);
	}
};

template< typename Arg >
BOOST_FORCEINLINE typename boost::proto::result_of::make_expr<boost::proto::tag::function, exp_fun, Arg>::type const exp(Arg arg)
{
	return boost::proto::make_expr<boost::proto::tag::function>(exp_fun(), arg);
}

struct tan_fun
{
	template <typename Sig>
	struct result;

	template <typename This, typename T>
	struct result<This(T)>
	{
		using type = T;

		BOOST_FORCEINLINE static type apply(const T& t) { using std::tan; return tan(t); }
	};

	template <typename This, typename Units, typename T>
	struct result<This(boost::units::quantity<Units, T>)>
	{
		using type = decltype(boost::units::tan(std::declval<boost::units::quantity<Units, T>>()));
		BOOST_FORCEINLINE static type apply(const boost::units::quantity<Units, T>& t) { using boost::units::tan; return tan(t); }
	};

	template <typename T>
	BOOST_FORCEINLINE typename result<tan_fun(T)>::type operator()(const T& d) const
	{
		return result<tan_fun(T)>::apply(d);
	}
};

template< typename Arg >
BOOST_FORCEINLINE typename boost::proto::result_of::make_expr<boost::proto::tag::function, tan_fun, Arg>::type const tan(Arg arg)
{
	return boost::proto::make_expr<boost::proto::tag::function>(tan_fun(), arg);
}

struct sqrt_fun
{
	template <typename Sig>
	struct result;

	template <typename This, typename T>
	struct result<This(T)>
	{
		using type = T;

		BOOST_FORCEINLINE static type apply(const T& t) { using std::sqrt; using std::abs; return sqrt(abs(t)); }
	};

	template <typename This, typename Units, typename T>
	struct result<This(boost::units::quantity<Units, T>)>
	{
		using type = decltype(boost::units::root<2>(std::declval<boost::units::quantity<Units, T>>()));
		BOOST_FORCEINLINE static type apply(const boost::units::quantity<Units, T>& t) { using boost::units::abs; return boost::units::root<2>(abs(t)); }
	};

	template <typename T>
	BOOST_FORCEINLINE typename result<sqrt_fun(T)>::type operator()(const T& d) const
	{
		return result<sqrt_fun(T)>::apply(d);
	}
};

template< typename Arg >
BOOST_FORCEINLINE typename boost::proto::result_of::make_expr<boost::proto::tag::function, sqrt_fun, Arg>::type const sqrt(Arg arg)
{
	return boost::proto::make_expr<boost::proto::tag::function>(sqrt_fun(), arg);
}

struct log_fun
{
	template <typename Sig>
	struct result;

	template <typename This, typename T>
	struct result<This(T)>
	{
		using type = T;

		BOOST_FORCEINLINE static type apply(const T& t) { using std::log; return log(t); }
	};

	template <typename This, typename Units, typename T>
	struct result<This(boost::units::quantity<Units, T>)>
	{
		using type = decltype(boost::units::log(std::declval<boost::units::quantity<Units, T>>()));
		BOOST_FORCEINLINE static type apply(const boost::units::quantity<Units, T>& t) { using boost::units::log; return log(t); }
	};

	template <typename T>
	BOOST_FORCEINLINE typename result<log_fun(T)>::type operator()(const T& d) const
	{
		return result<log_fun(T)>::apply(d);
	}
};

template< typename Arg >
BOOST_FORCEINLINE typename boost::proto::result_of::make_expr<boost::proto::tag::function, log_fun, Arg>::type const log(Arg arg)
{
	return boost::proto::make_expr<boost::proto::tag::function>(log_fun(), arg);
}

template <typename T>
struct evaluation_context : boost::proto::callable_context<evaluation_context<T> const> {
	T value;

	explicit evaluation_context(T value)
		: value(value)
	{}

	template <typename Sig>
	struct result;

	template <typename This, typename Tag, int V>
	struct result<This(Tag, derivative_constant<V>)>
	{
		using type = double;
	};

	template <typename This, typename Tag, int V>
	struct result<This(Tag, const derivative_constant<V>&)>
	{
		using type = double;
	};

	template <typename This, typename Tag>
	struct result<This(Tag, x_var)>
	{
		using type = T;
	};

	template <typename This, typename Tag>
	struct result<This(Tag, const x_var&)>
	{
		using type = T;
	};

	template <int V>
	BOOST_FORCEINLINE double operator()(boost::proto::tag::terminal, derivative_constant<V>) const
	{
		return V;
	}

	BOOST_FORCEINLINE T operator()(boost::proto::tag::terminal, x_var) const
	{
		return value;
	}
};

template<typename Expr>
struct derivative_expr : boost::proto::extends<Expr, derivative_expr<Expr>, domain> 
{
	typedef boost::proto::extends<Expr, derivative_expr<Expr>, domain> base_type;

	derivative_expr(Expr const& expr = Expr())
		: base_type(expr)
	{}

	template <typename Sig>
	struct result;

	template <typename This, typename T>
	struct result<This(T)>
	{
		using raw_type = typename boost::proto::result_of::eval<Expr, evaluation_context<T>>::type;
		using type = typename boost::remove_const<typename boost::remove_reference<raw_type>::type>::type;
	};
	
	template <typename T>
	BOOST_FORCEINLINE typename result<derivative_expr(T)>::type operator()(T d) const
	{
		evaluation_context<T> context(d);
		return boost::proto::eval(*this, context);
	}
};

//! Constants
struct constant_rule : boost::proto::callable
{
	using result_type = derivative_expr <boost::proto::terminal<derivative_constant<0>>::type>;

	BOOST_FORCEINLINE result_type operator()() const
	{
		return result_type();
	}
};

//! Derivative Variable
struct derivative_variable_rule : boost::proto::callable
{
	using result_type = derivative_expr <boost::proto::terminal<derivative_constant<1>>::type>;

	BOOST_FORCEINLINE result_type operator()() const
	{
		return result_type();
	}
};

//! Addition

namespace derivative_detail {

	template<typename Tag>
	struct make_optimized_binary
	{
		BOOST_PROTO_CALLABLE();
		BOOST_PROTO_POLY_FUNCTION();

		using Domain = boost::proto::domainns_::deduce_domain;

		template<typename Sig>
		struct result;

		template<typename This, typename A0, typename A1>
		struct result<This(A0, A1)>
		{
			using raw_type = typename boost::proto::result_of::make_expr<Tag, Domain, A0, A1>::type;

			using type = typename boost::result_of<optimize(raw_type)>::type;
		};

		/// Construct an expression node with tag type \c Tag
		/// and in the domain \c Domain.
		///
		/// \return <tt>proto::make_expr\<Tag, Domain\>(a0,...aN)</tt>
		template<typename A0, typename A1>
		BOOST_FORCEINLINE
		typename result<make_optimized_binary<Tag>(A0,A1)>::type const operator ()(A0 const &a0, A1 const& a1) const
		{
			auto result = optimize()(boost::proto::detail::make_expr_<Tag, Domain, A0 const, A1 const>()(a0, a1));
			//boost::proto::display_expr(result, std::cout);
			return result;
		}

		/// INTERNAL ONLY
		///
		template<typename A0, typename A1>
		struct impl
			: boost::proto::detail::make_expr_<Tag, Domain, A0, A1>
		{};
	};
		
	struct optimize_cases_
	{
		template <typename Tag, int D = 0>
		struct case_ : boost::proto::not_<boost::proto::_> {};

		template <int D>
		struct case_<boost::proto::tag::plus, D>
			: boost::proto::or_
			  <
				boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::plus, boost::proto::_, boost::proto::terminal<derivative_constant<0>> >
				,	optimize(boost::proto::_left)
				>
				, boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::plus, boost::proto::terminal<derivative_constant<0>>, boost::proto::_ >
				,	optimize(boost::proto::_right)
				>
				, boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::plus, boost::proto::_, boost::proto::_ >
				,	make_optimized_binary<boost::proto::tag::plus>(optimize(boost::proto::_left), optimize(boost::proto::_right))
				>
				, boost::proto::otherwise<boost::proto::_expr>
			  >
		{};

		template <int D>
		struct case_<boost::proto::tag::minus, D>
			: boost::proto::or_
			  <
				boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::minus, boost::proto::_, boost::proto::terminal<derivative_constant<0>> >
				,	optimize(boost::proto::_left)
				>
				, boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::minus, boost::proto::terminal<derivative_constant<0>>, boost::proto::_ >
				,	boost::proto::_make_negate(optimize(boost::proto::_right))
				>
				, boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::minus, boost::proto::_, boost::proto::_ >
				,	boost::proto::call<make_optimized_binary<boost::proto::tag::minus>(optimize(boost::proto::_left), optimize(boost::proto::_right))>
				>
				, boost::proto::otherwise<boost::proto::_expr>
			  >
		{};
		
		template <int D>
		struct case_<boost::proto::tag::multiplies, D>
			: boost::proto::or_
			  <
				boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::multiplies, boost::proto::_, boost::proto::terminal<derivative_constant<0>> >
				,	constant_rule()
				>
				, boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::multiplies, boost::proto::terminal<derivative_constant<0>>, boost::proto::_ >
				,	constant_rule()
				>
				, boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::multiplies, boost::proto::_, boost::proto::terminal<derivative_constant<1>> >
				,	optimize(boost::proto::_left)
				>
				, boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::multiplies, boost::proto::terminal<derivative_constant<1>>, boost::proto::_ >
				,	optimize(boost::proto::_right)
				>
				, boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::multiplies, boost::proto::_, boost::proto::_ >
				,	boost::proto::call<make_optimized_binary<boost::proto::tag::multiplies>(optimize(boost::proto::_left), optimize(boost::proto::_right))>
				>
				, boost::proto::otherwise<boost::proto::_expr>
			  >
		{};

		template <int D>
		struct case_<boost::proto::tag::divides, D>
			: boost::proto::or_
			  <
				boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::divides, boost::proto::terminal<derivative_constant<0>>, boost::proto::_ >
				,	constant_rule()
				>
				, boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::divides, boost::proto::_, boost::proto::terminal<derivative_constant<1>> >
				,	optimize(boost::proto::_left)
				>
				, boost::proto::when
				<
					boost::proto::binary_expr< boost::proto::tag::divides, boost::proto::_, boost::proto::_ >
				,	boost::proto::call<make_optimized_binary<boost::proto::tag::divides>(optimize(boost::proto::_left), optimize(boost::proto::_right))>
				>
				, boost::proto::otherwise<boost::proto::_expr>
			  >
		{};
		
		template <int D>
		struct case_<boost::proto::tag::function, D>
			: boost::proto::when
		      <
				boost::proto::_
			  , boost::proto::_make_function(boost::proto::_left, optimize(boost::proto::_right))
			  >
		{};
	};

	template <typename T1, typename T2>
	struct plus
	{
		using type = decltype(std::declval<T1>() + std::declval<T2>());
	};

	template <typename T1>
	struct plus<T1, derivative_constant<0>>
	{
		using type = T1;
	};

	template <typename T2>
	struct plus<derivative_constant<0>, T2>
	{
		using type = T2;
	};

}//! namespace derivative_detail;

namespace boost {
	namespace proto {
		template<typename Tag>
		struct is_callable<derivative_detail::make_optimized_binary<Tag> >
			: mpl::true_
		{};
} }

struct addition_rule : boost::proto::callable
{
	template <typename Sig>
	struct result;

	template <typename This, typename T1, typename T2>
	struct result<This(T1, T2)>
	{
	private:
		using t1_t = typename boost::remove_const<typename boost::remove_reference<T1>::type>::type;
		using t2_t = typename boost::remove_const<typename boost::remove_reference<T2>::type>::type;

	public:
		using raw_type = typename derivative_detail::plus<t1_t, t2_t>::type;
		using type = typename boost::result_of<derivative_detail::optimize(raw_type)>::type;
	};

	template <typename T1, typename T2>
	typename result<addition_rule(T1, T2)>::type operator()(const T1& t1, const T2& t2) const
	{
		return derivative_detail::optimize()(t1 + t2);
	}
};

//! Subtraction

namespace derivative_detail {

	template <typename T1, typename T2>
	struct minus
	{
		using type = decltype(std::declval<T1>() - std::declval<T2>());
	};

	template <typename T1>
	struct minus<T1, derivative_constant<0>>
	{
		using type = T1;
	};

	template <typename T2>
	struct minus<derivative_constant<0>, T2>
	{
		using type = decltype(-std::declval<T2>());
	};

}//! namespace derivative_detail;

struct subtraction_rule : boost::proto::callable
{
	template <typename Sig>
	struct result;

	template <typename This, typename T1, typename T2>
	struct result<This(T1, T2)>
	{
	private:
		using t1_t = typename boost::remove_const<typename boost::remove_reference<T1>::type>::type;
		using t2_t = typename boost::remove_const<typename boost::remove_reference<T2>::type>::type;

	public:
		using raw_type = typename derivative_detail::minus<t1_t, t2_t>::type;
		using type = typename boost::result_of<derivative_detail::optimize(raw_type)>::type;
	};

	template <typename T1, typename T2>
	typename result<subtraction_rule(T1, T2)>::type operator()(const T1& t1, const T2& t2) const
	{
		return derivative_detail::optimize()(t1 - t2);
	}
};

//! Multiplication
namespace derivative_detail {

	template <typename T1, typename T2>
	struct multiplies
	{
		using type = decltype(std::declval<T1>() * std::declval<T2>());
	};

	template <typename T1>
	struct multiplies<T1, derivative_constant<0>>
	{
		using type = derivative_constant<0>;
	};

	template <typename T2>
	struct multiplies<derivative_constant<0>, T2>
	{
		using type = derivative_constant<0>;
	};
	
	template <typename T1>
	struct multiplies<T1, derivative_constant<1>>
	{
		using type = T1;
	};

	template <typename T2>
	struct multiplies<derivative_constant<1>, T2>
	{
		using type = T2;
	};

	template <typename Arg1, typename dArg1, typename Arg2, typename dArg2>
	struct product_rule_result_helper
	{
		using termA = decltype(std::declval<dArg1>() * std::declval<Arg2>());
		using termB = decltype(std::declval<Arg1>() * std::declval<dArg2>());
		using otermA = typename boost::result_of<optimize(termA)>::type;
		using otermB = typename boost::result_of<optimize(termB)>::type;
		using type = decltype(std::declval<otermA>() + std::declval<otermB>());
	};
	
}//! namespace derivative_detail;

struct product_rule : boost::proto::callable
{
	template <typename Sig>
	struct result;

	template <typename This, typename Arg1, typename dArg1, typename Arg2, typename dArg2>
	struct result<This(Arg1, dArg1, Arg2, dArg2)>
	{
	private:

		using arg1_t = typename boost::remove_const<typename boost::remove_reference<Arg1>::type>::type;
		using darg1_t = typename boost::remove_const<typename boost::remove_reference<dArg1>::type>::type;
		using arg2_t = typename boost::remove_const<typename boost::remove_reference<Arg2>::type>::type;
		using darg2_t = typename boost::remove_const<typename boost::remove_reference<dArg2>::type>::type;
		using raw_type = typename derivative_detail::product_rule_result_helper<arg1_t, darg1_t, arg2_t, darg2_t>::type;

	public:
		using type = typename boost::result_of<derivative_detail::optimize(raw_type)>::type;
	};

	template <typename Arg1, typename dArg1, typename Arg2, typename dArg2>
	typename result<product_rule(Arg1, dArg1, Arg2, dArg2)>::type operator()(const Arg1& a1, const dArg1& da1, const Arg2& a2, const dArg2& da2) const
	{
		using result_type = typename result<product_rule(Arg1, dArg1, Arg2, dArg2)>::type;
		derivative_detail::optimize opt;
		auto result = opt(opt(da1 * a2) + opt(a1 * da2));
		return result;
	}
};

//! Division
struct quotient_rule : boost::proto::callable
{
	template <typename Sig>
	struct result;

	template <typename This, typename Arg1, typename dArg1, typename Arg2, typename dArg2>
	struct result<This(Arg1, dArg1, Arg2, dArg2)>
	{
	private:
		using termA_t = decltype(std::declval<dArg1>() * std::declval<Arg2>());
		using otermA_t = typename boost::result_of<derivative_detail::optimize(termA_t)>::type;
		using termB_t = decltype(std::declval<dArg2>() * std::declval<Arg1>());
		using otermB_t = typename boost::result_of<derivative_detail::optimize(termB_t)>::type;

		using num_t = typename boost::result_of<derivative_detail::optimize(decltype(std::declval<otermA_t>() - std::declval<otermB_t>()))>::type;
		using denom_t = typename boost::result_of<derivative_detail::optimize(decltype(std::declval<Arg2>() * std::declval<Arg2>()))>::type;
		using raw_type = decltype(std::declval<num_t>() / std::declval<denom_t>());
	public:

		using type = typename boost::result_of<derivative_detail::optimize(raw_type)>::type;
	};

	template <typename Arg1, typename dArg1, typename Arg2, typename dArg2>
	typename result<quotient_rule(Arg1, dArg1, Arg2, dArg2)>::type operator()(const Arg1& a1, const dArg1& da1, const Arg2& a2, const dArg2& da2) const
	{
		using result_type = typename result<quotient_rule(Arg1, dArg1, Arg2, dArg2)>::type;
		derivative_detail::optimize opt;
		auto result = opt( opt(opt(da1 * a2) - opt(da2 * a1)) / opt(a2 * a2) );
		return result;
	}
};

namespace derivative_detail
{
	template <typename Fn, typename Arg, typename dArg>
	struct chain_rule_result_helper;

	template <int Exp, typename Arg, typename dArg>
	struct chain_rule_result_helper<pow_fun<Exp>, Arg, dArg>
	{
		using type = decltype(static_cast<double>(Exp) * pow<Exp - 1>(std::declval<Arg>()) * std::declval<dArg>());

		static type apply(const Arg& arg, const dArg& darg)
		{
			return static_cast<double>(Exp) * pow<Exp - 1>(arg) * darg;
		}
	};

	template <typename Arg, typename dArg>
	struct chain_rule_result_helper<pow_fun<2>, Arg, dArg>
	{
		using type = decltype(2 * std::declval<Arg>() * std::declval<dArg>());

		static type apply(const Arg& arg, const dArg& darg)
		{
			return 2 * arg * darg;
		}
	};

	template <typename Arg, typename dArg>
	struct chain_rule_result_helper<pow_fun<1>, Arg, dArg>
	{
		using type = dArg;

		static type apply(const Arg& arg, const dArg& darg)
		{
			return darg;
		}
	};

	template <typename Arg, typename dArg>
	struct chain_rule_result_helper<cos_fun, Arg, dArg>
	{
		using type = decltype( -1.0 * sin( std::declval<Arg>() ) * std::declval<dArg>() );

		static type apply(const Arg& arg, const dArg& darg)
		{
			using std::sin;
			return -1.0 * sin(arg) * darg;
		}
	};
	
	template <typename Arg, typename dArg>
	struct chain_rule_result_helper<sin_fun, Arg, dArg>
	{
		using type = decltype(cos(std::declval<Arg>()) * std::declval<dArg>());

		static type apply(const Arg& arg, const dArg& darg)
		{
			using std::cos;
			return cos(arg) * darg;
		}
	};

	template <typename Arg, typename dArg>
	struct chain_rule_result_helper<exp_fun, Arg, dArg>
	{
		using type = decltype(exp(std::declval<Arg>()) * std::declval<dArg>());

		static type apply(const Arg& arg, const dArg& darg)
		{
			using std::exp;
			return exp(arg) * darg;
		}
	};

	template <typename Arg, typename dArg>
	struct chain_rule_result_helper<tan_fun, Arg, dArg>
	{
		using type = decltype(sec(std::declval<Arg>()) * sec(std::declval<Arg>()) * std::declval<dArg>());

		static type apply(const Arg& arg, const dArg& darg)
		{
			using std::cos;
			auto r = cos(arg);
			return (1.0 / (r * r)) * darg;
		}
	};

	template <typename Arg, typename dArg>
	struct chain_rule_result_helper<log_fun, Arg, dArg>
	{
		using type = decltype( (1.0 / std::declval<Arg>()) * std::declval<dArg>());

		static type apply(const Arg& arg, const dArg& darg)
		{
			using std::log;
			return (1.0 / arg) * darg;
		}
	};
	
	template <typename Arg, typename dArg>
	struct chain_rule_result_helper<sqrt_fun, Arg, dArg>
	{
		using type = decltype((0.5 / sqrt(std::declval<Arg>())) * std::declval<dArg>());

		static type apply(const Arg& arg, const dArg& darg)
		{
			using std::sqrt;
			return (0.5 / sqrt(arg)) * darg;
		}
	};

}//! namespace derivative_detail;

//! Chain rule
struct chain_rule : boost::proto::callable
{
	template <typename Sig>
	struct result;

	template <typename This, typename Fn, typename Arg, typename dArg>
	struct result<This(Fn, Arg, dArg)>
	{
	private:
		using fn_t = typename boost::remove_const<typename boost::remove_reference<Fn>::type>::type;
		using arg_t = typename boost::remove_const<typename boost::remove_reference<Arg>::type>::type;
		using darg_t = typename boost::remove_const<typename boost::remove_reference<dArg>::type>::type;
		using raw_type = typename derivative_detail::chain_rule_result_helper<fn_t, arg_t, darg_t>::type;

	public:

		using type = typename boost::result_of<derivative_detail::optimize(raw_type)>::type;
	};

	template <typename Fn, typename Arg, typename dArg>
	typename result<chain_rule(Fn, Arg, dArg)>::type operator() (const Fn&, const Arg& arg, const dArg& darg) const
	{
		auto result = derivative_detail::optimize()(derivative_detail::chain_rule_result_helper<Fn, Arg, dArg>::apply(arg, darg));
		//boost::proto::display_expr(result, std::cout);
		return result;
	}
};

struct get_value
	: boost::proto::when<boost::proto::terminal<boost::proto::_>, boost::proto::_value>
{};

struct derivative_grammar
	: boost::proto::switch_<struct cases_>
{};

struct cases_
{
	template <typename Tag, int D=0>
	struct case_ : boost::proto::not_<boost::proto::_> {};

	template <int D>
	struct case_<boost::proto::tag::terminal, D>
		: boost::proto::or_
		  <
			boost::proto::when
			<
				boost::proto::terminal<x_var>
			,	derivative_variable_rule()
			>
		  , boost::proto::when
			<
				boost::proto::terminal<boost::proto::_>
			,	constant_rule()
			>
		  >
	{};

	template <int D>
	struct case_<boost::proto::tag::function, D>
		: boost::proto::when
		  <
			boost::proto::_
		  ,	chain_rule(get_value(boost::proto::_left), boost::proto::_right, derivative_grammar(boost::proto::_right))
		  >
	{};

	template <int D>
	struct case_<boost::proto::tag::divides, D>
		: boost::proto::when
		  <
			boost::proto::_
		  ,	quotient_rule(boost::proto::_left, derivative_grammar(boost::proto::_left), boost::proto::_right, derivative_grammar(boost::proto::_right))
		  >
	{};

	template <int D>
	struct case_<boost::proto::tag::multiplies, D>
		: boost::proto::when
		  <
			boost::proto::_
		  ,	product_rule(boost::proto::_left, derivative_grammar(boost::proto::_left), boost::proto::_right, derivative_grammar(boost::proto::_right))
		  >
	{};

	template <int D>
	struct case_<boost::proto::tag::plus, D>
		: boost::proto::when
		  <
			boost::proto::_
		  ,	addition_rule(derivative_grammar(boost::proto::_left), derivative_grammar(boost::proto::_right))
		  >
	{};

	template <int D>
	struct case_<boost::proto::tag::minus, D>
		: boost::proto::when
		  <
			boost::proto::_
		  ,	subtraction_rule(derivative_grammar(boost::proto::_left), derivative_grammar(boost::proto::_right))
		  >
	{};
};
