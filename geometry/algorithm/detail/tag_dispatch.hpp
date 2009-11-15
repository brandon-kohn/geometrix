//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_TAG_DISPATCH_HPP
#define GENERATIVE_GEOMETRY_TAG_DISPATCH_HPP
#pragma once

#include <boost\typeof\typeof.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{

    //! \def GG_TAG_DISPATCH_REGISTER_IMPLEMENTATION
    //! \brief Macro to define a lookup function used to resolve a specific
    //! algorithm implementation from a tag.
    #define GG_TAG_DISPATCH_REGISTER_IMPLEMENTATION( ALGORITHM_NAMESPACE, TAG, IMPL )\
    namespace ALGORITHM_NAMESPACE                                                     \
    {                                                                                 \
        inline IMPL LookupImplementation( TAG )                                       \
        {                                                                             \
            return IMPL();                                                            \
        }                                                                             \
    }                                                                                 \
    /***/

    //! \def GG_TAG_DISPATCH_RESOLVE_IMPL
    //! \brief A macro to simplify resolving an algorithm implementation type from a tag.
    //! Example usage:
    //! \code
    //! // The DispatchHelper is a type to resolve the type for an implementation
    //! // of an algorithm from the category mapping keyed on the type T.
    //! template <typename T>
    //! struct DispatchHelper
    //! {
    //!     typedef typename DispatchTag<T>::type AlgorithmTag;
    //! 
    //!     // Use ADL on category tags to look up an implementation for this algorithm object.
    //!     typedef GG_TAG_DISPATCH_RESOLVE_IMPL( Algorithm, AlgorithmTag ) Implementation;
    //! };
    //! \endcode
    #define GG_TAG_DISPATCH_RESOLVE_IMPL( ALGORITHM_NAMESPACE, TAG )             \
    typename BOOST_TYPEOF( ALGORITHM_NAMESPACE::LookupImplementation( TAG() ) ) ) \
    /***/

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_TAG_DISPATCH_HPP
