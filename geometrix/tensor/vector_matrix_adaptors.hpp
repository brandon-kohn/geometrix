//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_MATRIX_ADAPTOR_HPP
#define GEOMETRIX_VECTOR_MATRIX_ADAPTOR_HPP

#include <geometrix/tensor/matrix_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/tensor/matrix.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>

namespace geometrix {

    template <typename Vector>
    struct column_vector
    {        
        explicit column_vector( Vector& v )
            : v(v)            
        {}
        
        Vector& v;
    };

    template <typename Vector>
    inline column_vector<Vector> as_column_vector( Vector& v )
    {
        return column_vector<Vector>(v);
    }

    template <typename Vector>
    struct geometric_traits< column_vector< Vector >, typename geometric_traits<Vector>::is_homogeneous > 
    {
        BOOST_STATIC_ASSERT( is_vector<Vector>::value );
        typedef typename dimension_of<Vector>::type              row_dimension;
        typedef dimension<1>                                     col_dimension;
        typedef void                                             is_matrix;
        typedef void                                             is_homogeneous;
    };                  

    template <typename Vector>
    struct geometric_traits< column_vector< Vector >, typename geometric_traits<Vector>::is_heterogeneous > 
    {
        BOOST_STATIC_ASSERT( is_vector<Vector>::value );
        typedef typename dimension_of<Vector>::type              row_dimension;
        typedef dimension<1>                                     col_dimension;
        typedef void                                             is_matrix;
        typedef void                                             is_heterogeneous;
    };                  

    //! Row/Column
    template <typename Vector> 
    struct column<column_vector<Vector>, 0>
    {
        typedef boost::mpl::int_<0> index;
        explicit column( column_vector<Vector> v )                                                                                      
            : v(v.v)                                                                                                                        
        {};         

        template <std::size_t Index>
        struct type_at
            : boost::mpl::eval_if
              <
                  is_homogeneous<Vector>
                , boost::fusion::result_of::at_c< Vector, 0 >
                , boost::fusion::result_of::at_c< Vector, Index >
              >
        {};

        template <std::size_t Row>                                                                                                       
        typename type_at<Row>::type get() const  
        {                                                                                                             
            return geometrix::get<Row>( v );                                                                     
        }                  

        template <std::size_t Row>
        void set( const typename type_at<Row>::type& value ) 
        {
            return geometrix::set<Row, index::value>(v,value);
        }
                                                                                                                                      
        Vector& v;      
    };
    
    template <typename Vector>
    struct is_vector< column<column_vector<Vector>,0> > : boost::false_type{};

    template <typename Vector>
    struct geometric_traits< column<column_vector<Vector>,0> > : geometric_traits<Vector>
    {};                  
    
    template <typename Vector, std::size_t Row> 
    struct row<column_vector<Vector>,Row>
    {
        typedef boost::mpl::int_<Row> index;
        explicit row( column_vector<Vector> v )
            : v(v.v)
        {};

        template <std::size_t Index>
        struct type_at
            : boost::mpl::eval_if
                <
                    is_homogeneous<Vector>
                  , boost::fusion::result_of::at_c< Vector, 0 >
                  , boost::fusion::result_of::at_c< Vector, Index >
                >
        {};
        
        template <std::size_t Column>
        typename type_at<index::value>::type get() const
        {
            return geometrix::get<index::value>( v );                                           
        }                                                        
                                                                                                                                                      
        Vector& v;
    };

    template <typename Vector, std::size_t Index>
    struct is_vector< row<column_vector<Vector>,Index> > : boost::false_type{};

    template <typename Vector, std::size_t Index>
    struct geometric_traits< row<column_vector<Vector>,Index> > : geometric_traits<Vector>
    {};
        
    template <typename Vector>
    struct column_vector_access_policy
    {        
        template <std::size_t Row, std::size_t Column>
        struct type_at
            : boost::mpl::eval_if
                <
                    is_homogeneous<Vector>
                  , boost::fusion::result_of::at_c< Vector, 0 >
                  , boost::fusion::result_of::at_c< Vector, Row >
                >
        {
            BOOST_STATIC_ASSERT( Column == 0 );            
        };

        template <std::size_t Row, std::size_t Column>
        static typename type_at< Row, Column >::type get( const column_vector<Vector>& matrix ) 
        {
            BOOST_STATIC_ASSERT( Column == 0 );
            return geometrix::get<Row>( matrix.v );
        }
             
        template <std::size_t Row, std::size_t Column>
        static void set( column_vector<Vector>& col, const typename type_at<Row, Column>::type& v ) 
        {
            BOOST_STATIC_ASSERT(( Column == 0 ));
            geometrix::set<Row>(col.v, v);
        }
    };
    template <typename Vector>
    struct tensor_traits< column_vector<Vector> >
    {
        typedef column_vector_access_policy< Vector > access_policy;
        typedef boost::mpl::int_<2>                   tensor_order;
        typedef void                                  rank_2;
        typedef void                                  is_tensor;
        typedef void make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
    };

    //! Row Vector
    template <typename Vector>
    struct row_vector
    {        
        explicit row_vector( Vector& v )
            : v(v)            
        {}
        
        Vector& v;
    };

    template <typename Vector>
    inline row_vector<Vector> as_row_vector( Vector& v )
    {
        return row_vector<Vector>(v);
    }
    
    template <typename Vector>
    struct geometric_traits< row_vector< Vector >, typename geometric_traits<Vector>::is_homogeneous > 
    {
        typedef typename dimension_of<Vector>::type              col_dimension;
        typedef dimension<1>                                     row_dimension; 
        typedef void                                             is_matrix;
        typedef void                                             is_homogeneous;
    };                  

    template <typename Vector>
    struct geometric_traits< row_vector< Vector >, typename geometric_traits<Vector>::is_heterogeneous > 
    {
        typedef typename dimension_of<Vector>::type              col_dimension;
        typedef dimension<1>                                     row_dimension;    
        typedef void                                             is_matrix;
        typedef void                                             is_heterogeneous;
    };   

    //! Row/Column
    template <typename Vector> 
    struct row<row_vector<Vector>, 0>
    {
        typedef boost::mpl::int_<0> index;
        explicit row( row_vector<Vector> v )                                                                                      
            : v(v.v)                                                                                                                        
        {};      

        template <std::size_t Index>
        struct type_at
            : boost::mpl::eval_if
                <
                    is_homogeneous<Vector>
                  , boost::fusion::result_of::at_c< Vector, 0 >
                  , boost::fusion::result_of::at_c< Vector, Index >
                >
        {};
                                                                                                                           
        template <std::size_t Column>                                                                                                       
        typename type_at<Column>::type get() const  
        {                                                                                                                                 
            return geometrix::get<Column>( v );                                  
        }                                                                                                                                 
                                                                                                                                      
        Vector& v;
    };

    template <typename Vector>
    struct geometric_traits< row<row_vector<Vector>,0> > : geometric_traits<Vector>{};

    template <typename Vector>
    struct is_vector< row<row_vector<Vector>,0> > : boost::false_type{};
        
    template <typename Vector, std::size_t Column> 
    struct column<row_vector<Vector>,Column>
    {
        typedef boost::mpl::int_<Column> index;
        explicit column( row_vector<Vector> v )
            : v(v.v)
        {};

        template <std::size_t Index>
        struct type_at
            : boost::mpl::eval_if
                <
                    is_homogeneous<Vector>
                  , boost::fusion::result_of::at_c< Vector, 0 >
                  , boost::fusion::result_of::at_c< Vector, Index >
                >
        {};

        template <std::size_t Row>
        typename type_at<Row>::type get() const
        {
            return geometrix::get<Column>( v );
        }
                                                                                                   
        Vector& v;
    };
        
    template <typename Vector, std::size_t Index>
    struct geometric_traits< column<row_vector<Vector>,Index> > : geometric_traits<Vector>{};

    template <typename Vector, std::size_t Index>
    struct is_vector< column<row_vector<Vector>,Index> > : boost::false_type{};
    
    template <typename Vector>
    struct row_vector_access_policy
    {
        template <std::size_t Row, std::size_t Column>
        struct type_at
            : boost::mpl::eval_if
                <
                    is_homogeneous<Vector>
                  , boost::fusion::result_of::at_c< Vector, 0 >
                  , boost::fusion::result_of::at_c< Vector, Column >
                >
        {
            BOOST_STATIC_ASSERT( Row == 0 );
        };

        template <std::size_t Row, std::size_t Column>
        static typename type_at<Row, Column>::type get( const row_vector<Vector>& matrix ) 
        {
            BOOST_STATIC_ASSERT( Row == 0 );
            return geometrix::get<Column>( matrix.v );
        }

        template <std::size_t Row, std::size_t Column>
        static void set( row_vector<Vector>& matrix, const typename type_at<Row,Column>::type& v ) 
        {
            BOOST_STATIC_ASSERT(( Row == 0 ));
            geometrix::set<Column>( matrix.v, v );
        }
    };

    template <typename Vector>
    struct tensor_traits< row_vector<Vector> >
    {
        typedef row_vector_access_policy< Vector > access_policy;
        typedef boost::mpl::int_<2>                tensor_order;
        typedef void                               rank_2;
        typedef void                               is_tensor;
        typedef void make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
    };

}//namespace geometrix;

//! Mark the expr as an MPL type sequence.
namespace boost { namespace mpl {
template<typename>                              
struct sequence_tag;                            
                                                
template<typename T>                            
struct sequence_tag< geometrix::row_vector<T> >
{                                               
    typedef fusion::fusion_sequence_tag type;   
};                                              
template<typename T>             
struct sequence_tag<geometrix::row_vector<T> const>
{                                               
    typedef fusion::fusion_sequence_tag type;   
};                                              

template<typename T>                            
struct sequence_tag< geometrix::column_vector<T> >
{                                               
    typedef fusion::fusion_sequence_tag type;   
};                                              
template<typename T>             
struct sequence_tag<geometrix::column_vector<T> const>
{                                               
    typedef fusion::fusion_sequence_tag type;   
};                                         

}}//namespace boost::mpl
    
#endif//GEOMETRIX_VECTOR_MATRIX_ADAPTOR_HPP
