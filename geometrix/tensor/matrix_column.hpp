//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MATRIX_COLUMN_HPP
#define GEOMETRIX_MATRIX_COLUMN_HPP

#include <geometrix/geometric_traits.hpp>
#include <geometrix/tensor/tensor_access_policy.hpp>

#include <boost/mpl/int.hpp>

namespace geometrix {

template <typename Matrix, std::size_t Column, typename EnableIf=void>
struct column
{
    typedef boost::mpl::int_<Column>                index;
    typedef typename remove_const_ref<Matrix>::type matrix_type;

    explicit column( const Matrix& m )
        : m(m)
    {}

    template <std::size_t Index>
    struct type_at
        : boost::mpl::eval_if
          <
              is_homogeneous< column<matrix_type,index::value> >
            , boost::mpl::at_c< typename geometric_traits< column<matrix_type, index::value> >::storage_types, 0 >
            , boost::mpl::at_c< typename geometric_traits< column<matrix_type, index::value> >::storage_types, Index >
          >
    {};

    template <std::size_t Row>
    typename type_at<Row>::type get() const
    {
        return geometrix::get<Row, index::value>( m );
    }

    template <std::size_t Row>
    void set( const typename type_at<Row>::type& v )
    {
        geometrix::set<Row,index::value>(m,v);
    }

    const Matrix& m;
};

template <typename Slice>
struct matrix_slice_access_policy;

template <typename Matrix, std::size_t Column>
struct matrix_slice_access_policy< column<Matrix, Column> >
{
    typedef boost::mpl::int_<Column>                column_index;
    typedef typename remove_const_ref<Matrix>::type matrix_type;

    template <std::size_t Index, typename EnableIf=void>
    struct type_at
    {
        typedef typename column<matrix_type, column_index::value>::template type_at<Index>::type type;
    };

    template <std::size_t Row>
    static typename type_at<Row>::type get( const column<Matrix,Column>& col )
    {
        return geometrix::get<Row, Column>( col.m );
    }

    template <std::size_t Row>
    static void set( column<Matrix,Column>& col, const typename type_at<Row>::type& v )
    {
        geometrix::set<Row,Column>(col.m,v);
    }
};

template <typename Matrix, std::size_t Column>
struct tensor_traits< column<Matrix,Column> >
{
    typedef matrix_slice_access_policy< column<Matrix,Column> > access_policy;
    typedef boost::mpl::int_<1>                                 tensor_order;
    typedef void                                                rank_1;
    typedef void                                                is_tensor;
    typedef void make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
};

}//namespace geometrix;

//! Mark the column as an MPL type sequence.
namespace boost { namespace mpl {

template<typename>
struct sequence_tag;

template<typename T, std::size_t C>
struct sequence_tag< geometrix::column<T,C> >
{
    typedef fusion::fusion_sequence_tag type;
};
template<typename T, std::size_t C>
struct sequence_tag<geometrix::column<T,C> const>
{
    typedef fusion::fusion_sequence_tag type;
};
}}//namespace boost::mpl

#endif//GEOMETRIX_MATRIX_COLUMN_HPP
