//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MATRIX_ROW_HPP
#define GEOMETRIX_MATRIX_ROW_HPP

#include <geometrix/geometric_traits.hpp>
#include <geometrix/tensor/tensor_access_policy.hpp>

#include <boost/mpl/int.hpp>
#include <boost/mpl/at.hpp>
#include <boost/fusion/sequence.hpp>

namespace geometrix {

template <typename Matrix, std::size_t Row, typename EnableIf=void>
struct row
{
    typedef typename remove_const_ref<Matrix>::type matrix_type;
    typedef boost::mpl::int_<Row>                   index;

    explicit row( const Matrix& m )
        : m(m)
    {};

    template <std::size_t Index>
    struct type_at
        : boost::mpl::eval_if
          <
              is_homogeneous< row<matrix_type,index::value> >
            , boost::mpl::at_c< typename geometric_traits< row<matrix_type, index::value> >::storage_types, 0>
            , boost::mpl::at_c< typename geometric_traits< row<matrix_type, index::value> >::storage_types, Index >
          >
    {};

    template <std::size_t Column>
    typename type_at<Column>::type get() const
    {
        return geometrix::get<index::value, Column>( m );
    }

    template <std::size_t Column>
    void set( const typename type_at<Column>::type& v )
    {
        geometrix::set<index::value,Column>(m,v);
    }

    const Matrix& m;
};

template <typename Slice>
struct matrix_slice_access_policy;

template <typename Matrix, std::size_t Row>
struct matrix_slice_access_policy< row<Matrix, Row> >
{
    typedef boost::mpl::int_<Row>                   row_index;
    typedef typename remove_const_ref<Matrix>::type matrix_type;

    template <std::size_t Index, typename EnableIf=void>
    struct type_at
    {
        typedef typename row<Matrix, Row>::template type_at<Index>::type type;
    };

    template <std::size_t Column>
    static typename type_at<Column>::type get( const row<Matrix,Row>& r )
    {
        return geometrix::get<Row, Column>( r.m );
    }

    template <std::size_t Column>
    static void set( row<Matrix,Row>& r, typename type_at<Column>::type const& v )
    {
        r.template set<Column>(v);
    }
};

template <typename Matrix, std::size_t Row>
struct tensor_traits< row<Matrix,Row> >
{
    typedef matrix_slice_access_policy< row<Matrix,Row> > access_policy;
    typedef boost::mpl::int_<1>                           tensor_order;
    typedef void                                          rank_1;
    typedef void                                          is_tensor;
    typedef void make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
};

}//namespace geometrix;

//! Mark the row as an MPL type sequence.
namespace boost { namespace mpl {
template<typename>
struct sequence_tag;

template<typename T, std::size_t R>
struct sequence_tag< geometrix::row<T,R> >
{
    typedef fusion::fusion_sequence_tag type;
};
template<typename T, std::size_t R>
struct sequence_tag<geometrix::row<T,R> const>
{
    typedef fusion::fusion_sequence_tag type;
};
}}//namespace boost::mpl

#endif//GEOMETRIX_MATRIX_ROW_HPP
