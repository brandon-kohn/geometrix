//
//! Copyright © 2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TAGGED_QUANTITY_HPP
#define GEOMETRIX_TAGGED_QUANTITY_HPP

#include <geometrix/arithmetic/arithmetic.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <geometrix/arithmetic/scalar_arithmetic.hpp>

namespace geometrix {

template<typename Tag, typename Y>
class tagged_quantity
{
public:

    using this_type = tagged_quantity<Tag, Y>;
    using value_type = Y;

    tagged_quantity() = default;

    explicit tagged_quantity(const value_type& value)
        : m_val(value)
    {}

    //! implicit conversion between value types is allowed if allowed for value types themselves
    template<typename YY>
    tagged_quantity(const tagged_quantity<Tag, YY>& source) 
        : m_val(source.value())
    {
        static_assert(boost::is_convertible<YY, Y>::value, "type is not convertible to value_type");
    }

    //! implicit assignment between value types is allowed if allowed for value types themselves
    template<typename YY>
    this_type& operator=(const tagged_quantity<Tag, YY>& source)
    {
        static_assert(boost::is_convertible<YY, Y>::value, "type is not convertible to value_type");
        *this = this_type(source);
        return *this;
    }

    template <typename YY>
    this_type& operator=(const YY& value)
    {
        static_assert(boost::is_convertible<YY, Y>::value, "type is not convertible to value_type");
        m_val = value;
        return *this;
    }
        
    const value_type& value() const { return m_val; }

    template<typename YY>
    this_type& operator+=(const tagged_quantity<Tag, YY>& source)
    {
        m_val += source.value();
        return *this;
    }

    template<typename YY>
    this_type& operator-=(const tagged_quantity<Tag, YY>& source)
    {
        m_val -= source.value();
        return *this;
    }

    template<typename YY>
    this_type& operator*=(const tagged_quantity<Tag, YY>& source)
    {
        m_val *= source.value();
        return *this;
    }

    template<typename YY>
    this_type& operator/=(const tagged_quantity<Tag, YY>& source)
    {
        m_val /= source.value();
        return *this;
    }
	        
    this_type& operator+=(const value_type& source) { m_val += source; return *this; }
    this_type& operator-=(const value_type& source) { m_val -= source; return *this; }
    this_type& operator*=(const value_type& source) { m_val *= source; return *this; }
    this_type& operator/=(const value_type& source) { m_val /= source; return *this; }

    //! Allow implicit conversion to value_type
    //operator const value_type& () const { return m_val; }

    explicit operator bool() const { return m_val; }

    template <typename YY>
    void swap(YY& rhs)
    {
        using std::swap;
        static_assert(boost::is_convertible<YY, Y>::value, "type is not convertible to value_type");
        swap(m_val, rhs);
    }

    template <typename YY>
    void swap(tagged_quantity<Tag, YY>& rhs)
    {
        using std::swap;
        static_assert(boost::is_convertible<YY, Y>::value, "type is not convertible to value_type");
        swap(m_val, rhs.m_val);
    }

private:

    value_type m_val;

};

template <typename Tag, typename Numeric>
struct construction_policy< tagged_quantity< Tag, Numeric > >
{
	template <typename T>
	static tagged_quantity< Tag, Numeric > construct(T&& n) { return tagged_quantity< Tag, Numeric >(boost::numeric_cast<Numeric>(geometrix::get(n))); }
};

template <typename Tag, typename Numeric>
struct construction_policy< const tagged_quantity< Tag, Numeric > >
{
	template <typename T>
	static const tagged_quantity< Tag, Numeric > construct(T&& n) { return tagged_quantity< Tag, Numeric >(boost::numeric_cast<Numeric>(geometrix::get(n))); }
};

//! Define numeric traits for the coordinate type.
template <typename Tag, typename Numeric>
struct is_numeric< tagged_quantity< Tag, Numeric > > : boost::true_type {};

template <typename Tag, typename Numeric>
struct numeric_traits< tagged_quantity< Tag, Numeric > >
{
	typedef tagged_quantity< Tag, Numeric >      numeric_type;
	typedef typename boost::is_float< Numeric >::type    is_float;
	typedef typename boost::is_integral< Numeric >::type is_integral;
	typedef void                                         is_numeric;
	static const unsigned int digits = std::numeric_limits<Numeric>::digits10;
};

//! Arithmetic operators

template <typename T1, typename T2>
struct multiply;

template <typename T1, typename T2>
struct divide;

template <typename T1, typename T2>
struct add;

template <typename T1, typename T2>
struct subtract;

namespace detail {
    template <template <typename, typename> class Op, typename T1, typename T2, typename EnableIf = void>
    struct make_op_tag
    {
        using type = Op<T1, T2>;
    };

    template <template <typename, typename> class Op, typename Tag>
    struct make_op_tag<Op, Tag, Tag>
    {
        using type = Tag;
    };
}

#define GEOMETRIX_DEFINE_TAGGED_QUANTITY_BINARY_ARITHMETIC_OPERATOR(OpTag, Op)               \
template<typename Tag1, typename Tag2, typename X, typename Y>                               \
inline tagged_quantity                                                                       \
 <                                                                                           \
    typename detail::make_op_tag<OpTag, Tag1, Tag2>::type                                    \
  , decltype(std::declval<X>() Op std::declval<Y>())                                         \
 >                                                                                           \
 operator Op(const tagged_quantity<Tag1, X>& lhs, const tagged_quantity<Tag2, Y>& rhs)       \
{                                                                                            \
    using tag = typename detail::make_op_tag<OpTag, Tag1, Tag2>::type;                       \
    using type = tagged_quantity<tag, decltype(std::declval<X>() Op std::declval<Y>())>;     \
    return type(lhs.value() Op rhs.value());                                                 \
}                                                                                            \
template<typename Tag, typename X, typename Y>                                               \
inline tagged_quantity<Tag, decltype(std::declval<X>() Op std::declval<Y>())>                \
operator Op(const tagged_quantity<Tag, X>& lhs, const Y& rhs)                                \
{                                                                                            \
    typedef tagged_quantity<Tag, decltype(std::declval<X>() Op std::declval<Y>())> type;     \
    return type(lhs.value() Op rhs);                                                         \
}                                                                                            \
template<typename X, typename Tag, typename Y>                                               \
inline tagged_quantity<Tag, decltype(std::declval<X>() Op std::declval<Y>())>                \
operator Op(const X& lhs, const tagged_quantity<Tag, Y>& rhs)                                \
{                                                                                            \
    typedef tagged_quantity<Tag, decltype(std::declval<X>() Op std::declval<Y>())> type;     \
    return type(lhs Op rhs.value());                                                         \
}                                                                                            \
/***/

GEOMETRIX_DEFINE_TAGGED_QUANTITY_BINARY_ARITHMETIC_OPERATOR(add, +)
GEOMETRIX_DEFINE_TAGGED_QUANTITY_BINARY_ARITHMETIC_OPERATOR(subtract, -)
GEOMETRIX_DEFINE_TAGGED_QUANTITY_BINARY_ARITHMETIC_OPERATOR(multiply, *)
GEOMETRIX_DEFINE_TAGGED_QUANTITY_BINARY_ARITHMETIC_OPERATOR(divide, / )

template<class Tag, class Y>
inline tagged_quantity<Tag, Y> operator+(const tagged_quantity<Tag, Y>& val)
{
	return tagged_quantity<Tag, Y>(+val.value());
}

template<class Tag, class Y>
inline tagged_quantity<Tag, Y> operator-(const tagged_quantity<Tag, Y>& val)
{
	return tagged_quantity<Tag, Y>(-val.value());
}

//! Relational operators

#define GEOMETRIX_DEFINE_TAGGED_QUANTITY_BINARY_RELATIONAL_OPERATOR(Op)                      \
template<typename Tag, typename X, typename Y>                                               \
inline bool operator Op(const tagged_quantity<Tag, X>& lhs,const tagged_quantity<Tag, Y>&rhs)\
{                                                                                            \
    return lhs.value() Op rhs.value();                                                       \
}                                                                                            \
template<typename Tag, typename X>                                                           \
inline bool operator Op(const tagged_quantity<Tag, X>& lhs, const X& rhs)                    \
{                                                                                            \
    return lhs.value() Op rhs;                                                               \
}                                                                                            \
template<typename Tag, typename X>                                                           \
inline bool operator Op(const X& lhs, const tagged_quantity<Tag, X>& rhs)                    \
{                                                                                            \
    return lhs Op rhs.value();                                                               \
}                                                                                            \
/***/

GEOMETRIX_DEFINE_TAGGED_QUANTITY_BINARY_RELATIONAL_OPERATOR(==)
GEOMETRIX_DEFINE_TAGGED_QUANTITY_BINARY_RELATIONAL_OPERATOR(<)
GEOMETRIX_DEFINE_TAGGED_QUANTITY_BINARY_RELATIONAL_OPERATOR(<=)
GEOMETRIX_DEFINE_TAGGED_QUANTITY_BINARY_RELATIONAL_OPERATOR(>)
GEOMETRIX_DEFINE_TAGGED_QUANTITY_BINARY_RELATIONAL_OPERATOR(>=)
GEOMETRIX_DEFINE_TAGGED_QUANTITY_BINARY_RELATIONAL_OPERATOR(!=)

}//! namespace geometrix;

#define GEOMETRIX_STRONG_TYPEDEF(Type, Name)                \
struct Name ## Tag;                                         \
using Name = geometrix::tagged_quantity<Name ## Tag, Type>; \
/***/

#define GEOMETRIX_STRONG_TYPEDEF_TPL(Name)                  \
struct Name ## Tag;                                         \
template <typename Type>                                    \
using Name = geometrix::tagged_quantity<Name ## Tag, Type>; \
/***/

#endif //!GEOMETRIX_TAGGED_QUANTITY_HPP
