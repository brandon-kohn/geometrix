//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_COMPONENTS_INTERSECTION_HPP
#define GEOMETRIX_LINEAR_COMPONENTS_INTERSECTION_HPP


/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

enum intersection_type
{
    e_invalid_intersection = -1,
    e_non_crossing = 0,
    e_crossing = 1,
    e_overlapping = 2,
    e_endpoint = 3,
};

}//namespace geometrix;

#endif //GEOMETRIX_LINEAR_COMPONENTS_INTERSECTION_HPP
