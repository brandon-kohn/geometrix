//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_COORDINATE_BASIS_HPP
#define GEOMETRIX_COORDINATE_BASIS_HPP

#include <geometrix/space/coordinate_basis_traits.hpp>
#include <boost/mpl/vector.hpp>

namespace geometrix {  

    //! \brief Traits struct for specializing the properties of a coordinate basis.

    template < typename AffineSpace, typename CoordinateProperties >
    struct coordinate_basis
    {        
        typedef AffineSpace          space_type;   
        typedef CoordinateProperties coordinate_properties;
    };
      
}//namespace geometrix;

#endif //GEOMETRIX_COORDINATE_BASIS_HPP
