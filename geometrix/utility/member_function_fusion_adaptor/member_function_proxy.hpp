//
//! Copyright © 2008-2011
//! Brandon Kohn
//! Derived/copied from ADT example in Boost.Fusion. Thanks guys ;).
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_MEMBER_FUNCTION_PROXY_HPP
#define GEOMETRIX_DETAIL_MEMBER_FUNCTION_PROXY_HPP

namespace boost { namespace fusion { namespace extension {

    template <typename T, std::size_t Index, typename EnableIf=void>
    struct member_function_proxy
    {};

#define GEOMETRIX_DEFINE_MEMBER_FUNCTION_PROXY(T, Index, ReturnType, ConstReturnType, GetName, SetName ) \
    template <>                                                                                          \
    struct member_function_proxy<T, Index>                                                               \
    {                                                                                                    \
        typedef ConstReturnType const_return_type;                                                       \
        typedef ReturnType      return_type;                                                             \
                                                                                                         \
        member_function_proxy( boost::call_traits<T>::reference instance )                               \
            : instance( instance )                                                                       \
        {}                                                                                               \
                                                                                                         \
        template <typename V>                                                                            \
        void set( const V& v )                                                                           \
        {                                                                                                \
            instance.SetName(v);                                                                         \
        }                                                                                                \
                                                                                                         \
        operator ConstReturnType() const                                                                 \
        {                                                                                                \
            return instance.GetName();                                                                   \
        };                                                                                               \
                                                                                                         \
        operator ReturnType()                                                                            \
        {                                                                                                \
            return instance.GetName();                                                                   \
        };                                                                                               \
                                                                                                         \
        boost::call_traits<T>::reference instance;                                                       \
    };                                                                                                   \
    template <>                                                                                          \
    struct member_function_proxy<const T, Index>                                                         \
    {                                                                                                    \
        typedef ConstReturnType const_return_type;                                                       \
                                                                                                         \
        member_function_proxy( boost::call_traits<const T>::reference instance )                         \
            : instance( instance )                                                                       \
        {}                                                                                               \
                                                                                                         \
        operator ConstReturnType() const                                                                 \
        {                                                                                                \
            return instance.GetName();                                                                   \
        };                                                                                               \
                                                                                                         \
        boost::call_traits<const T>::reference instance;                                                 \
    };                                                                                                   \
/***/

}}}//namespace boost::fusion::extension;                              

#endif
