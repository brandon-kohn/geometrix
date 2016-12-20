//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_PRIVATEALLOCATOR_TESTS_HPP
#define GEOMETRIX_PRIVATEALLOCATOR_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <geometrix/utility/private_allocator.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>

class my_type
{
	//! These constructs allow the type to only be constructed and deleted via the boost::shared_ptr. Solid RAII.
	struct private_arg {};
	GEOMETRIX_PRIVATE_ALLOCATOR(private_alloc);
	friend struct private_alloc<my_type>;

public:

	//! This factory method is used to construct instances of GKCEPedestrianType.
	static boost::shared_ptr<my_type> create() { return boost::allocate_shared<my_type>(private_alloc<my_type>(), private_arg{}); }

	//! This constructor is used by create to allow make_shared to work, but cannot be used otherwise.
	my_type(private_arg) {}//! Allows make_shared to construct but no other access.

private:

	my_type() = default;
	~my_type(){}

};

BOOST_AUTO_TEST_CASE(TestPrivateAllocator)
{
	using namespace geometrix;

	auto ptr = my_type::create();

	BOOST_CHECK(ptr != nullptr);
}

#endif//! GEOMETRIX_PRIVATEALLOCATOR_TESTS_HPP
