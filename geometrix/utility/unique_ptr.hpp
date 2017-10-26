//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_UNIQUE_PTR_HPP
#define GEOMETRIX_UNIQUE_PTR_HPP

#if defined(_MSC_VER)
    #pragma once
#endif

#include <boost/config.hpp>

#if defined(BOOST_NO_CXX11_SMART_PTR)
#include <boost/move/unique_ptr.hpp>
#include <boost/move/make_unique.hpp>

namespace std
{
    //! boost::unique_ptr makes more sense for maintenence and eventually moving to std::unique_ptr. Hoisted it.
    using movelib::unique_ptr;
    using movelib::make_unique;
}
#else
    #include <memory>
    #include <boost/smart_ptr/make_unique.hpp>
#endif

#endif//GEOMETRIX_UNIQUE_PTR_HPP
