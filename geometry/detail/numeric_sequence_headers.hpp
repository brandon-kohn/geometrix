//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_DETAIL_NUMERIC_SEQUENCE_HEADERS_HPP
#define _GENERATIVE_GEOMETRY_DETAIL_NUMERIC_SEQUENCE_HEADERS_HPP
#pragma once

#include <cassert>
#include <algorithm>
#include <boost/config/no_tr1/utility.hpp>
#include <boost/config.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/inc.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/array.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/utility.hpp>
#include <boost/operators.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/detail/access.hpp>
#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/fusion/functional/adapter/fused.hpp>
#include <boost/fusion/functional/adapter/fused_procedure.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/array.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/accumulate.hpp>
#include <boost/fusion/view/zip_view.hpp>
#include <boost/type_traits/is_const.hpp>

#include "detail/numeric_sequence_base.hpp"
#include "dimension_traits.hpp"

#endif //_GENERATIVE_GEOMETRY_DETAIL_NUMERIC_SEQUENCE_HEADERS_HPP


