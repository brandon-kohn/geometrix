//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_POINT_BASE_HPP
#define GEOMETRIX_DETAIL_POINT_BASE_HPP

#include <boost/mpl/assert.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/detail/access.hpp>
#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/config/no_tr1/utility.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/config.hpp>

namespace geometrix {

    template <typename NumericType, unsigned int Dimension>
    class point 
    {
            BOOST_MPL_ASSERT_MSG( 
              ( false )
            , POINT_DIMENSION_NOT_DEFINED
            , (NumericType) );	
    };

    template <typename NumericType>
    class point<NumericType,0>
    {
        BOOST_MPL_ASSERT_MSG( 
              ( false )
            , POINT_CANNOT_HAVE_ZERO_DIMENSION
            , (NumericType) );	
    };

}//namespace geometrix;

//GEOMETRIX_DETAIL_POINT_BASE_HPP
#endif
