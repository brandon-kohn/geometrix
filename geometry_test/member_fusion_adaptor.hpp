//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MEMBER_FUNCTION_FUSION_ADAPTOR_HPP
#define GEOMETRIX_MEMBER_FUNCTION_FUSION_ADAPTOR_HPP


#include <geometrix/utility/member_function_fusion_adaptor.hpp>

#include <boost/test/included/unit_test.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/sequence/intrinsic.hpp>
#include <boost/fusion/sequence/io.hpp>

#include <iostream>

// A simple point structure to model a 3d point with multiple types.
class my_type
{
public:

    my_type( double x, int y, double z )
        : x( x )
        , y( y )
        , z( boost::lexical_cast<std::string>( z ) )
    {}

    double&            get_x() { return x; }
    const double&      get_x() const { return x; }

    int&               get_y() { return y; }
    const int&         get_y() const { return y; }
    
    std::string&       get_z() { return z; }
    const std::string& get_z() const { return z; }

    void set_x( const double& v ) { x = v; }
    void set_y( const int& v ) { y = v; }
    void set_z( const std::string& v ) { z = v; }

private:

    double      x;    
    int         y;
    std::string z;

};

// Adapt the struct into a fusion sequence to provide a compile time access interface.
GEOMETRIX_MEMBER_FUNCTION_FUSION_SEQUENCE
(
    my_type, 
    (double&, const double&, get_x, set_x)
    (int&, const int&,  get_y, set_y)
    (std::string&, const std::string&, get_z, set_z)
);

BOOST_AUTO_TEST_CASE( TestMemberFusionAdaptor )
{
    using namespace boost;
    using namespace boost::fusion;
    std::cout << typeid( fusion::result_of::at_c<my_type, 0>::type ).name() << std::endl;
    std::cout << typeid( fusion::result_of::at_c<my_type, 1>::type ).name() << std::endl;
    std::cout << typeid( fusion::result_of::at_c<my_type, 2>::type ).name() << std::endl;

    //! Const version.
    {
        const my_type mt( 1.0, 2, 3.0 );

        const double& x = fusion::at_c<0>( mt );
        const int& y = fusion::at_c<1>( mt );
        const std::string& z = fusion::at_c<2>( mt );
        std::cout << mt << std::endl;
    }

    //! Non-const version.
    {
        my_type mt( 1.0, 2, 3.0 );

        double& x = fusion::at_c<0>( mt );
        int& y = fusion::at_c<1>( mt );
        std::string& z = fusion::at_c<2>( mt );

        std::cout << mt.get_z() << std::endl;
        z = "modified!";
        std::cout << mt.get_z() << std::endl;
    }
}

#endif //GEOMETRIX_MEMBER_FUNCTION_FUSION_ADAPTOR_HPP
