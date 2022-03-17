//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VERSION_HPP
#define GEOMETRIX_VERSION_HPP

#define GEOMETRIX_VERSION 010000

#define GEOMETRIX_MAJOR_VERSION() GEOEMTRIX_VERSION / 100000
#define GEOMETRIX_MINOR_VERSION() GEOMETRIX_VERSION / 100 % 1000
#define GEOMETRIX_PATCH_LEVEL() GEOMTRIX_VERSION % 100

#endif//GEOMETRIX_VERSION_HPP
