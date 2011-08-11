#if !defined(GEOMETRIX_DONT_USE_PREPROCESSED_FILES)

    #include <geometrix/tensor/detail/preprocessed/matrix.hpp>

#elif !defined(BOOST_PP_IS_ITERATING)

    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/matrix.hpp")
    #endif
    //
    //! Copyright © 2008-2011
    //! Brandon Kohn
    //
    //  Distributed under the Boost Software License, Version 1.0. (See
    //  accompanying file LICENSE_1_0.txt or copy at
    //  http://www.boost.org/LICENSE_1_0.txt)
    //
    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define BOOST_PP_FILENAME_1 <geometrix/tensor/detail/matrix_m.hpp>
    #define BOOST_PP_ITERATION_LIMITS (1, GEOMETRIX_MAX_MATRIX_ROWS)
    #include BOOST_PP_ITERATE()

    namespace geometrix {

        template <typename N, unsigned int Rows, unsigned int Columns>
        struct construction_policy< matrix< N, Rows, Columns> >
        {    
            template <typename Matrix>                                                                         
            static matrix<N, Rows, Columns> construct( const Matrix& m )
            {                                                                                                           
                return detail::matrix_pod_constructor<Rows,Columns>::construct< matrix<N, Rows, Columns> >( m );
            }
        };

        template <typename N, unsigned int Rows, unsigned int Columns>
        struct geometric_traits< matrix<N,Rows,Columns> >  
        {
            typedef dimension<Rows>    row_dimension;
            typedef dimension<Columns> col_dimension;
            typedef void               is_matrix;
            typedef void               is_homogeneous;
        };    

        template <typename N, unsigned int Rows, unsigned int Columns, unsigned int Row>
        struct geometric_traits< row<matrix<N,Rows,Columns>,Row> >                                                    
        {
            typedef boost::mpl::vector<N> storage_types;
            typedef dimension<Columns>    dimension_type;
            typedef void                  is_sequence;
            typedef void                  is_numeric_sequence;
            typedef void                  is_vector;
            typedef void                  is_homogeneous;
        };                                                                                                                                                      

        template <typename N, unsigned int Rows, unsigned int Columns, unsigned int Column>
        struct geometric_traits< column<matrix<N,Rows,Columns>,Column> >                                                    
        {
            typedef boost::mpl::vector<N> storage_types;
            typedef dimension<Rows>       dimension_type;      
            typedef void                  is_sequence;         
            typedef void                  is_numeric_sequence; 
            typedef void                  is_vector;           
            typedef void                  is_homogeneous;                                                                                                            
        }; 

        template <typename N, unsigned int Rows, unsigned int Columns>
        struct tensor_traits< matrix<N, Rows, Columns> >
        {
            typedef index_operator_matrix_access_policy< matrix<N,Rows,Columns> > access_policy;
            typedef boost::mpl::int_<2>                                           tensor_order;
            typedef void                                                          rank_2;
            typedef void                                                          is_tensor;
        };
        
    }//namespace geometrix;

    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#endif
