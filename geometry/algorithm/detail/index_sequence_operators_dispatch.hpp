//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_OPERATORS_DISPATCH_HPP
#define GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_OPERATORS_DISPATCH_HPP
#pragma once

#include "indexed_access_traits.hpp"
//#include "detail/indexed_access_fusion_adaptor.hpp"
#include <boost/fusion/functional/adapter/fused_procedure.hpp>
#include <boost/utility.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{

//! Define the default implementation of the algorithm in an appropriately named namespace.
struct AdditionTag{};
namespace AdditionOperation
{
    // Define the implementation for the algorithm.
    struct Impl
    {        
        template <typename Left, typename Right>
        struct result
        {
            typedef BOOST_TYPEOF_TPL( Left() + Right() ) type;
        };

        // Implementation of algorithm logic.
        template <typename Left, typename Right>
        static typename result<Left,Right>::type Execute( const Left& lhs, const Right& rhs )
        {
            return lhs + rhs;
        }
    };
}

//! Register the algorithm with a compile-time lookup mechanism:
//! One way to do this is to implement a lookup function which is used to resolve a the implementation type from the algorithm tags
//! using <i>argument dependent lookup</i> (<b>ADL</b>). 
GGL_TAG_DISPATCH_REGISTER_IMPLEMENTATION( Operations, AdditionTag, AdditionOperation::Impl );

//! Define the default implementation of the algorithm in an appropriately named namespace.
struct SubtractionTag{};
namespace SubtractionOperation
{
    // Define the implementation for the algorithm.
    struct Impl
    {        
        template <typename Left, typename Right>
        struct result
        {
            typedef BOOST_TYPEOF_TPL( Left() - Right() ) type;
        };

        // Implementation of algorithm logic.
        template <typename Left, typename Right>
        static typename result<Left,Right>::type Execute( const Left& lhs, const Right& rhs )
        {
            return lhs - rhs;
        }
    };
}

//! Register the algorithm with a compile-time lookup mechanism:
//! One way to do this is to implement a lookup function which is used to resolve a the implementation type from the algorithm tags
//! using <i>argument dependent lookup</i> (<b>ADL</b>). 
GGL_TAG_DISPATCH_REGISTER_IMPLEMENTATION( Operations, SubtractionTag, SubtractionOperation::Impl );

struct MultiplicationTag{};
namespace MultiplicationOperation
{
    // Define the implementation for the algorithm.
    struct Impl
    {        
        template <typename Left, typename Right>
        struct result
        {
            typedef BOOST_TYPEOF_TPL( Left() * Right() ) type;
        };

        // Implementation of algorithm logic.
        template <typename Left, typename Right>
        static typename result<Left,Right>::type Execute( const Left& lhs, const Right& rhs )
        {
            return lhs * rhs;
        }
    };
}

//! Register the algorithm with a compile-time lookup mechanism:
//! One way to do this is to implement a lookup function which is used to resolve a the implementation type from the algorithm tags
//! using <i>argument dependent lookup</i> (<b>ADL</b>). 
GGL_TAG_DISPATCH_REGISTER_IMPLEMENTATION( Operations, MultiplicationTag, MultiplicationOperation::Impl );

struct UnaryNegationTag{};
namespace UnaryNegationOperation
{
    // Define the implementation for the algorithm.
    struct Impl
    {        
        template <typename T>
        struct result
        {
            typedef BOOST_TYPEOF_TPL( -(T()) ) type;
        };

        // Implementation of algorithm logic.
        template <typename T>
        static typename result<T>::type Execute( const T& rhs )
        {
            return -rhs;
        }
    };
}

//! Register the algorithm with a compile-time lookup mechanism:
//! One way to do this is to implement a lookup function which is used to resolve a the implementation type from the algorithm tags
//! using <i>argument dependent lookup</i> (<b>ADL</b>). 
GGL_TAG_DISPATCH_REGISTER_IMPLEMENTATION( Operations, UnaryNegationTag, UnaryNegationOperation::Impl );

//! Create a mechanism for associating an implementation tag with one or more types:
//! In this example this is done using a templated resolver type which defines the default implementation tag
//! for all types which do not specialize the resolver type to use another.
namespace Operations
{
    // A DispatchTag is a type specialization mechanism to map an algorithm implementation to the types which should be using it.
    // This struct maps an algorithm implementation to the category of types which should use it.

    // First define a base type which acts as the default.
    template <typename T, typename Op, typename Enable = void>
    struct UnaryOpTag
    {
        typedef Op type;
    };

    template <typename T1, typename T2, typename Op, typename Enable = void>
    struct BinaryOpTag
    {
        typedef Op type;
    };

    // Note: This can be extended as more algorithm implementations are added and the type mapping becomes
    // more complex.
}

//! Finally the dispatch framework is implemented in two parts:
//! The first part is a helper type to resolve the algorithm implementation type given a tag. 
//! This method uses the LookupImplementation function defined above and the Boost.Typeof library
//! to perform a compile-time mapping from the tag type (the argument to LookupImplementation) to
//! the implementation type (the return type of LookupImplementation.)
namespace Operations
{
    namespace Detail
    {
        // The DispatchHelper is a type to resolve the type for an implementation
        // of an algorithm from the category mapping keyed on the type T.
        template <typename T, typename Op>
        struct UnaryOpDispatcher
        {
            typedef typename UnaryOpTag<T, Op>::type OperationTag;

            // Use ADL on category tags to look up an implementation for this algorithm object.
            typedef typename BOOST_TYPEOF( Operations::LookupImplementation( OperationTag() ) ) Implementation;

            //! result type.
            typedef typename Implementation::result< T >::type result_type;
        };

        template <typename T1, typename T2, typename Op>
        struct BinaryOpDispatcher
        {
            typedef typename BinaryOpTag<T1, T2, Op>::type OperationTag;

            // Use ADL on category tags to look up an implementation for this algorithm object.
            typedef typename BOOST_TYPEOF( Operations::LookupImplementation( OperationTag() ) ) Implementation;

            //! result type.
            typedef typename Implementation::result< T1, T2 >::type result_type;
        };
    }
}

//! The second part is the implementation of the dispatch function. This function uses the DispatchHelper
//! to resolve the implementation type and then calls the appropriate interface.
namespace Operations
{
    // The algorithm dispatcher
    template <typename T, typename Op>
    typename Detail::UnaryOpDispatcher< T, Op >::result_type Dispatch( const T& t, Op )
    {
        // Resolve the implementation type from the helper and execute.
        return Detail::UnaryOpDispatcher< T, Op >::Implementation::Execute( t );
    }

    // The algorithm dispatcher
    template <typename Left, typename Right, typename Op>
    typename Detail::BinaryOpDispatcher< Left, Right, Op >::result_type Dispatch( const Left& lhs, const Right& rhs, Op )
    {
        // Resolve the implementation type from the helper and execute.
        return Detail::BinaryOpDispatcher< Left, Right, Op >::Implementation::Execute( lhs, rhs );
    }
}

}}}//namespace generative::numeric::geometry

#endif //GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_OPERATORS_DISPATCH_HPP
