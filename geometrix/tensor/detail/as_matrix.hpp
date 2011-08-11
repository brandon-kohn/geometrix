
#if !defined( GEOMETRIX_COMPOSITION_MAX_ROWS )
#define GEOMETRIX_COMPOSITION_MAX_ROWS 10
#endif            
//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(GEOMETRIX_COMPOSE_MATRIX_MAX_ARITY)
#define GEOMETRIX_COMPOSE_MATRIX_MAX_ARITY 10
#endif

#if !defined(GEOMETRIX_DONT_USE_PREPROCESSED_FILES)
#if !defined(GEOMETRIX_COMPOSE_MATRIX_DETAILS_HPP)
#define GEOMETRIX_COMPOSE_MATRIX_DETAILS_HPP
        #include <geometrix/tensor/detail/preprocessed/as_matrix.hpp>
#endif
#elif !defined(BOOST_PP_IS_ITERATING)
#if !defined(GEOMETRIX_COMPOSE_MATRIX_DETAILS_HPP)
#define GEOMETRIX_COMPOSE_MATRIX_DETAILS_HPP

    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/as_matrix.hpp")
    #endif
//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
namespace geometrix { 
            
        template <typename Sequence, unsigned int Rows, unsigned int Columns>
        struct composite_matrix;

        namespace result_of { namespace detail {

            template <unsigned int Min, unsigned int Max>
            struct interval
            {
                BOOST_STATIC_CONSTANT( unsigned int, lower_bound = Min ); 
                BOOST_STATIC_CONSTANT( unsigned int, upper_bound = Max );
            };

            template <typename Item, unsigned int Row, unsigned int Column>
            struct intersects
                : boost::mpl::bool_
                  <
                      Item::row_interval::lower_bound <= Row && Item::row_interval::upper_bound >= Row &&
                      Item::column_interval::lower_bound <= Column && Item::column_interval::upper_bound >= Column
                  >
            {};

            template <typename Item, unsigned int Column>
            struct column_intersects
                : boost::mpl::bool_
                  <
                      Item::column_interval::lower_bound <= Column && Item::column_interval::upper_bound >= Column
                  >
            {};

            template <typename Item, unsigned int RowOffset, unsigned int ColumnOffset, typename Index, typename RowItem = void>
            struct compose_element
            {
                template <unsigned int Row> struct translate_row: boost::mpl::int_<Row-RowOffset>{};
                template <unsigned int Col> struct translate_col: boost::mpl::int_<Col-ColumnOffset>{};

                typedef interval<RowOffset, RowOffset+geometrix::row_dimension_of<Item>::value-1>          row_interval;
                typedef interval<ColumnOffset, ColumnOffset+geometrix::column_dimension_of<Item>::value-1> column_interval;
                typedef Item                                                                    item_type;
                typedef Index                                                                   index;
            };
            
            template 
                <
                    typename Sequence                  
                  , typename TransformedSequence = boost::mpl::vector<>
                  , unsigned int RowIndex = 0
                  , unsigned int ColIndex = 0                  
                  , typename RowIter = boost::mpl::void_
                  , typename Row = boost::mpl::vector<>                  
                  , typename LastRow = boost::mpl::vector<> 
                  , unsigned int Index = 0
                  , unsigned int NextRow = RowIndex + GEOMETRIX_COMPOSITION_MAX_ROWS//! Set this to an effective infinitiy by default (new rows)
                >
            struct state
            {
                typedef boost::mpl::int_<RowIndex>             row_index; //! The current row index
                typedef boost::mpl::int_<ColIndex>             col_index; //! The current col index
                typedef boost::mpl::int_<NextRow>              next_row;  //! The min index for the next row.
                typedef RowIter                                row_iter;  //! The iterator in the last complete row.
                typedef Row                                    row;       //! The current row.
                typedef LastRow                                last_row;  //! The previous row.
                typedef boost::mpl::int_<Index>                index;     //! The index of the next item in the composition sequence.
                typedef Sequence                               sequence;
                typedef TransformedSequence                    result;
            };

            template 
                <
                    typename Sequence                  
                  , typename TransformedSequence = boost::mpl::vector<>
                  , unsigned int ColIndex = 0                  
                  , typename Row = boost::mpl::vector<>                  
                  , unsigned int Index = 0
                  , unsigned int NextRow = GEOMETRIX_COMPOSITION_MAX_ROWS//! Set this to an effective infinitiy by default (new rows)
                >
            struct first_state
                : state<Sequence, TransformedSequence, 0, ColIndex, boost::mpl::void_, Row, boost::mpl::vector<>, Index, NextRow>
            {
                typedef void is_first_row;
            };

            template <typename Sequence>
            struct default_state 
                : state<Sequence>
            {
                typedef void is_first_row;
            };

            template <typename State, unsigned int Columns>
            struct add_row_item
                : boost::mpl::if_c
                  <
                      boost::mpl::deref<typename State::row_iter>::type::column_interval::upper_bound + 1 == Columns
                    , state
                      <
                          typename State::sequence
                        , typename State::result
                        , boost::mpl::min
                          <
                              typename State::next_row
                            , boost::mpl::int_<boost::mpl::deref<typename State::row_iter>::type::row_interval::upper_bound + 1>
                          >::type::value
                        , 0
                        , typename boost::mpl::begin
                          <
                              typename boost::mpl::push_back
                              <
                                  typename State::row
                                , typename boost::mpl::deref<typename State::row_iter>::type
                              >::type
                          >::type
                        , boost::mpl::vector<>
                        , typename boost::mpl::push_back
                          <
                              typename State::row
                            , typename boost::mpl::deref<typename State::row_iter>::type
                          >::type
                        , State::index::value
                      >
                    , state
                      <
                          typename State::sequence
                        , typename State::result
                        , State::row_index::value
                        , boost::mpl::deref<typename State::row_iter>::type::column_interval::upper_bound + 1
                        , typename boost::mpl::next<typename State::row_iter>::type
                        , typename boost::mpl::push_back
                          <
                              typename State::row
                            , typename boost::mpl::deref<typename State::row_iter>::type
                          >::type
                        , typename State::last_row
                        , State::index::value
                        , boost::mpl::min
                          <
                              typename State::next_row
                            , boost::mpl::int_< boost::mpl::deref<typename State::row_iter>::type::row_interval::upper_bound + 1 >
                          >::type::value
                      >
                  >::type
            {};

            template <typename State, unsigned int Columns, typename EnableIf=void>
            struct add_new_item
                : boost::mpl::if_c
                  <
                      State::col_index::value + geometrix::column_dimension_of<typename boost::mpl::front<typename State::sequence>::type>::value == Columns
                    , state
                      <
                          typename boost::mpl::pop_front<typename State::sequence>::type
                        , typename boost::mpl::push_back
                          <
                              typename State::result
                            , compose_element<typename boost::mpl::front<typename State::sequence>::type, State::row_index::value, State::col_index::value, typename State::index>
                          >::type
                        , boost::mpl::min
                          <
                              typename State::next_row
                            , boost::mpl::int_< State::row_index::value + geometrix::row_dimension_of<typename boost::mpl::front<typename State::sequence>::type>::value >
                          >::type::value
                        , 0
                        , typename boost::mpl::begin
                          <
                              typename boost::mpl::push_back
                              <
                                  typename State::row
                                , compose_element<typename boost::mpl::front<typename State::sequence>::type, State::row_index::value, State::col_index::value, typename State::index>
                              >::type
                          >::type
                        , boost::mpl::vector<>
                        , typename boost::mpl::push_back
                          <
                              typename State::row
                            , compose_element<typename boost::mpl::front<typename State::sequence>::type, State::row_index::value, State::col_index::value, typename State::index>
                          >::type                        
                        , State::index::value + 1
                      >
                    , state
                      <
                          typename boost::mpl::pop_front<typename State::sequence>::type
                        , typename boost::mpl::push_back
                          <
                              typename State::result
                            , compose_element<typename boost::mpl::front<typename State::sequence>::type, State::row_index::value, State::col_index::value, typename State::index>
                          >::type
                        , State::row_index::value
                        , State::col_index::value + geometrix::column_dimension_of<typename boost::mpl::front<typename State::sequence>::type>::value
                        , typename boost::mpl::if_
                          <
                              column_intersects
                              <
                                  typename boost::mpl::deref<typename State::row_iter>::type
                                , State::col_index::value + geometrix::column_dimension_of<typename boost::mpl::front<typename State::sequence>::type>::value
                              >
                            , typename State::row_iter
                            , typename boost::mpl::next<typename State::row_iter>::type
                          >::type
                        , typename boost::mpl::push_back
                          <
                              typename State::row
                            , compose_element<typename boost::mpl::front<typename State::sequence>::type, State::row_index::value, State::col_index::value, typename State::index>
                          >::type
                        , typename State::last_row
                        , State::index::value + 1
                        , boost::mpl::min
                          <
                              typename State::next_row
                            , boost::mpl::int_< State::row_index::value + geometrix::row_dimension_of<typename boost::mpl::front<typename State::sequence>::type>::value >
                          >::type::value
                      >
                  >::type
            {};

            template <typename State, unsigned int Columns>
            struct add_new_item
                <
                      State
                    , Columns
                    , typename State::is_first_row
                >
                : boost::mpl::if_c
                  <
                      State::col_index::value + geometrix::column_dimension_of<typename boost::mpl::front<typename State::sequence>::type>::value == Columns
                    , state
                      <
                          typename boost::mpl::pop_front<typename State::sequence>::type
                        , typename boost::mpl::push_back
                          <
                              typename State::result
                            , compose_element
                              <
                                  typename boost::mpl::front<typename State::sequence>::type
                                , 0
                                , State::col_index::value
                                , typename State::index 
                              >
                          >::type
                        , boost::mpl::min
                          <
                              typename State::next_row
                            , boost::mpl::int_
                              <
                                  geometrix::row_dimension_of
                                  <
                                      typename boost::mpl::front<typename State::sequence>::type
                                  >::value
                              >
                          >::type::value
                        , 0
                        , typename boost::mpl::begin
                          <
                              typename boost::mpl::push_back
                              <
                                  typename State::row
                                , compose_element
                                  <
                                      typename boost::mpl::front<typename State::sequence>::type
                                    , 0
                                    , State::col_index::value
                                    , typename State::index 
                                  >
                              >::type
                          >::type
                        , boost::mpl::vector<>
                        , typename boost::mpl::push_back
                          <
                              typename State::row
                            , compose_element
                              <
                                  typename boost::mpl::front<typename State::sequence>::type
                                , 0
                                , State::col_index::value
                                , typename State::index 
                              >
                          >::type
                        , State::index::value + 1
                      >
                    , first_state
                      <
                          typename boost::mpl::pop_front<typename State::sequence>::type
                        , typename boost::mpl::push_back
                          <
                              typename State::result
                            , compose_element
                              <
                                  typename boost::mpl::front<typename State::sequence>::type
                                , 0
                                , State::col_index::value
                                , typename State::index 
                              >
                          >::type
                        , State::col_index::value + geometrix::column_dimension_of<typename boost::mpl::front<typename State::sequence>::type>::value
                        , typename boost::mpl::push_back
                          <
                              typename State::row
                            , compose_element<typename boost::mpl::front<typename State::sequence>::type, 0, State::col_index::value, typename State::index>
                          >::type
                        , State::index::value + 1
                        , boost::mpl::min
                          <
                              typename State::next_row
                            , boost::mpl::int_<geometrix::row_dimension_of<typename boost::mpl::front<typename State::sequence>::type>::value>
                          >::type::value
                      >
                  >::type
            {};

            template <typename State, unsigned int Columns>
            struct process_state
                : boost::mpl::if_
                  <
                      boost::mpl::or_
                      <
                          boost::mpl::empty<typename State::last_row>
                        , boost::mpl::not_
                          <
                              intersects
                              <
                                  typename boost::mpl::deref< typename State::row_iter >::type
                                , State::row_index::value
                                , State::col_index::value 
                              >
                          >
                      >
                    , add_new_item<State,Columns> 
                    , add_row_item<State,Columns> 
                  >
            {};

            template <typename State, unsigned int Columns>
            struct recurse_schema;
            
            template <typename State, unsigned int Columns>
            struct process_schema
                : boost::mpl::eval_if
                  <
                      boost::mpl::empty<typename State::sequence>
                    , boost::mpl::identity<typename State::result>
                    , recurse_schema<State,Columns>
                  >
            {
                BOOST_STATIC_ASSERT((State::col_index::value <= Columns));
            };

            template <typename State, unsigned int Columns>
            struct recurse_schema
                : process_schema
                  <
                      typename process_state<State,Columns>::type
                    , Columns
                  >
            {};

            template <typename Sequence, unsigned int Columns>
            struct compose_matrix_schema
                : process_schema< default_state<Sequence>, Columns >
            {};       
        
        }//namespace detail 
        
        template <unsigned int Rows, unsigned int Columns>
        struct compose_matrix_helper
        {
            template <typename Sig>
            struct result{};

            #define BOOST_PP_LOCAL_MACRO(n)                              \
            template <typename This, BOOST_PP_ENUM_PARAMS(n,typename A)> \
            struct result<This(BOOST_PP_ENUM_PARAMS(n,A))>               \
            {                                                            \
                typedef composite_matrix                                 \
                <                                                        \
                    boost::mpl::vector<BOOST_PP_ENUM_PARAMS(n,A)>        \
                  , Rows                                                 \
                  , Columns                                              \
                > type;                                                  \
            };                                                           \
            /***/

    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

            #define BOOST_PP_LOCAL_LIMITS (2, GEOMETRIX_COMPOSE_MATRIX_MAX_ARITY)
            #include BOOST_PP_LOCAL_ITERATE()

            
    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2)
    #endif

        };

        #define GEOMETRIX_NARY_ARGS_ENABLE_IF_(z, n, data) \
            typename BOOST_PP_CAT(A,n) = void              \
        /***/

        template 
            <
                unsigned int Rows
              , unsigned int Columns
              , BOOST_PP_ENUM
                (
                    GEOMETRIX_COMPOSE_MATRIX_MAX_ARITY
                  , GEOMETRIX_NARY_ARGS_ENABLE_IF_
                  , _
                )
              , typename EnableIf=void
            >
        struct as_matrix{};

        #undef GEOMETRIX_NARY_ARGS_ENABLE_IF_

    }//namespace result_of;

    
    #define GEOMETRIX_NARY_HOMOGENEOUS_ENABLE_IF_(z, n, data) \
        typename BOOST_PP_CAT(IsHomogeneous,n) = void         \
    /***/

    template 
        <
            typename Sequence
          , BOOST_PP_ENUM
            (
                GEOMETRIX_COMPOSE_MATRIX_MAX_ARITY
              , GEOMETRIX_NARY_HOMOGENEOUS_ENABLE_IF_
              , _
            )
        >
    struct composite_diversity_base
    {
        typedef void is_heterogeneous;
    };

    #undef GEOMETRIX_NARY_HOMOGENEOUS_ENABLE_IF_

    template <typename Sequence, unsigned int Rows, unsigned int Columns>
    struct composite_matrix
    {
        BOOST_MPL_ASSERT_MSG
        (
            (false)
          , CANNOT_COMPOSE_MATRIX_FROM_INPUT_SEQUENCE
          , (Sequence)
        );
    };

    template <typename Sequence, unsigned int Rows, unsigned int Columns>
    struct geometric_traits< composite_matrix<Sequence, Rows, Columns> >
        : composite_diversity_base<Sequence>
    {
        typedef dimension<Rows>    row_dimension;
        typedef dimension<Columns> col_dimension;
        typedef void               is_matrix;
    };

    template <typename Matrix>
    struct composite_matrix_access_policy
    {
        typedef void compile_time_access;

        template <unsigned int Row, unsigned int Column>
        struct type_at
            : Matrix::type_at<Row,Column>
        {
            BOOST_MPL_ASSERT_MSG
            (
                ( row_dimension_of< Matrix >::value > Row && column_dimension_of< Matrix >::value > Column )
              , MATRIX_INDICES_ARE_OUT_OF_BOUNDS
              , (type_at<Row,Column>)
            );           
        };

        template <unsigned int Row, unsigned int Column>
        static typename type_at<Row, Column>::type get( const Matrix& matrix ) 
        {
            return matrix.get<Row,Column>();
        }

        template <unsigned int Row, unsigned int Column>
        static void set( Matrix& matrix, const typename type_at<Row, Column>::type& v ) 
        {
            return matrix.set<Row,Column>(v);
        }
    };

    template <typename Sequence, unsigned int Rows, unsigned int Columns>
    struct tensor_traits< composite_matrix<Sequence, Rows, Columns> >
    {
        typedef composite_matrix_access_policy< composite_matrix<Sequence,Rows,Columns> > access_policy;
        typedef boost::mpl::int_<2>                                                       tensor_order;
        typedef void                                                                      rank_2;
        typedef void                                                                      is_tensor;
    };

    template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Column>
    struct column< composite_matrix<Sequence, Rows, Columns>, Column >                       
    {                                                                                                                                     
        typedef boost::mpl::int_<Column>                 index;
        typedef composite_matrix<Sequence,Rows, Columns> matrix_type;
    
        explicit column( const matrix_type& m )
            : m(m)
        {}

        template <unsigned int Row>
        struct type_at
            : matrix_type::type_at<Row,index::value>
        {};
                                                                                                                                      
        template <unsigned int Row>                                                                                                       
        typename type_at<Row>::type get() const  
        {                                                                                                                                 
            return geometrix::get<Row, index::value>( m );                                                                                
        }                                                                                                                                 
                                                                                                                                      
        matrix_type m;                                                                                               
    };

    template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Column>
    struct geometric_traits< column< composite_matrix<Sequence, Rows, Columns>, Column > >
        : composite_diversity_base<Sequence>
    {
        typedef dimension<Rows> dimension_type;
        typedef void            is_sequence;
        typedef void            is_numeric_sequence;
        typedef void            is_vector;  
    };

    template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Column>
    struct tensor_traits< column<composite_matrix<Sequence, Rows, Columns>, Column> >
    {
        typedef matrix_slice_access_policy< column<composite_matrix<Sequence,Rows,Columns>, Column> > access_policy;
        typedef boost::mpl::int_<1>                                 tensor_order;
        typedef void                                                rank_1;
        typedef void                                                is_tensor;
        typedef void make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
    };

    template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Row>
    struct row< composite_matrix<Sequence, Rows, Columns>, Row >                       
    {                                                                                                                                     
        typedef boost::mpl::int_<Row>                     index;
        typedef composite_matrix<Sequence, Rows, Columns> matrix_type;
    
        explicit row( const matrix_type& m )
            : m(m)
        {}

        template <unsigned int Column>
        struct type_at
            : matrix_type::type_at<index::value,Column>
        {};
                                                                                                                                      
        template <unsigned int Column>                                                                                                       
        typename type_at<Column>::type get() const  
        {                                                                                                                                 
            return geometrix::get<index::value, Column>( m );                                                                                
        }                                                                                                                                 
                                                                                                                                      
        matrix_type m;                                                                                               
    };

    template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Row>
    struct geometric_traits< row< composite_matrix<Sequence, Rows, Columns>, Row > >
        : composite_diversity_base<Sequence>
    {
        typedef dimension<Columns> dimension_type;
        typedef void               is_sequence;
        typedef void               is_numeric_sequence;
        typedef void               is_vector;  
    };

    template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Row>
    struct tensor_traits< row<composite_matrix<Sequence, Rows, Columns>, Row> >
    {
        typedef matrix_slice_access_policy< row<composite_matrix<Sequence,Rows,Columns>,Row> > access_policy;
        typedef boost::mpl::int_<1>                                 tensor_order;
        typedef void                                                rank_1;
        typedef void                                                is_tensor;
        typedef void make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
    };
    
    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define BOOST_PP_ITERATION_PARAMS_1                                                            \
        (3, (2, GEOMETRIX_COMPOSE_MATRIX_MAX_ARITY, <geometrix/tensor/detail/as_matrix.hpp>))
    #include BOOST_PP_ITERATE()
        
    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2)
    #endif

    }//namespace geometrix;

    //! Mark the column as an MPL type sequence.
    namespace boost { namespace mpl {
        template<typename>                              
        struct sequence_tag;                            
                                                
        template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Row>
        struct sequence_tag< geometrix::row<geometrix::composite_matrix<Sequence, Rows, Columns>, Row> >
        {                                               
            typedef fusion::fusion_sequence_tag type;   
        };                                              
        template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Row>
        struct sequence_tag< geometrix::row<geometrix::composite_matrix<Sequence, Rows, Columns>, Row> const >
        {                                               
            typedef fusion::fusion_sequence_tag type;   
        };                                              
        template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Column>
        struct sequence_tag< geometrix::column<geometrix::composite_matrix<Sequence, Rows, Columns>, Column> >
        {                                               
            typedef fusion::fusion_sequence_tag type;   
        };                                              
        template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Column>
        struct sequence_tag< geometrix::column<geometrix::composite_matrix<Sequence, Rows, Columns>, Column> const >
        {                                               
            typedef fusion::fusion_sequence_tag type;   
        };                                              
    }}//namespace boost::mpl

    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif      

    #endif// GEOMETRIX_DETAIL_COMPOSE_MATRIX_HPP 
#else

    #define N BOOST_PP_ITERATION()

    #define GEOMETRIX_REF_WRAPPER( z, n, T ) boost::reference_wrapper< BOOST_PP_CAT(T,n) >

    #define GEOMETRIX_REF( z, n, T ) boost::ref(BOOST_PP_CAT(T,n))

    #define GEOMETRIX_NARY_IS_HOMOGENEOUS_(z, n, data)                                                \
        typename geometric_traits<typename remove_const_ref<BOOST_PP_CAT(A,n)>::type>::is_homogeneous \
    /***/

    namespace result_of {
    
        template <unsigned int Rows, unsigned int Columns, BOOST_PP_ENUM_PARAMS(N,typename A)>
        struct as_matrix
            <
                Rows
              , Columns
              , BOOST_PP_ENUM_PARAMS(N,A)
            >
            : boost::result_of
              <
                  result_of::compose_matrix_helper<Rows,Columns>
                  (
                      BOOST_PP_ENUM_PARAMS(N,A)
                  )
              >    
        {};

    }//namespace result_of;

    template <BOOST_PP_ENUM_PARAMS(N,typename A)>
    struct composite_diversity_base
        <
            boost::mpl::vector<BOOST_PP_ENUM_PARAMS(N,A)>
          , BOOST_PP_ENUM
            (
                N
              , GEOMETRIX_NARY_IS_HOMOGENEOUS_
              , _
            )
        >
    {
        //! TODO: Until a check for each type in the sequence is done, keep this hetero.
        typedef void is_heterogeneous;
    };
    
    template <BOOST_PP_ENUM_PARAMS(N, typename A), unsigned int Rows, unsigned int Columns>
    struct composite_matrix<boost::mpl::vector<BOOST_PP_ENUM_PARAMS(N,A)>, Rows, Columns>
    {
    private:

        struct schema 
            : result_of::detail::compose_matrix_schema
              <
                  boost::mpl::vector<BOOST_PP_ENUM_PARAMS(N,A)>
                , Columns
              >::type 
        {};

    public:

        composite_matrix( BOOST_PP_ENUM_BINARY_PARAMS(N, A, &a) )
            : sequence( BOOST_PP_ENUM(N, GEOMETRIX_REF, a) )
        {}

        template <unsigned int Row, unsigned int Column>
        struct type_at
        {
        private:
            
            struct predicate
            {
                template <typename Item>
                struct apply
                    : boost::mpl::bool_
                      <
                          Item::row_interval::lower_bound <= Row && Item::row_interval::upper_bound >= Row &&
                          Item::column_interval::lower_bound <= Column && Item::column_interval::upper_bound >= Column
                      >
                {};
            };

            struct sub_matrix 
                : boost::mpl::deref
                  <
                      typename boost::mpl::find_if
                      <
                         schema
                       , predicate
                      >::type 
                  >::type
            {};

        public:

            BOOST_STATIC_CONSTANT( unsigned int, index = sub_matrix::index::value );
            BOOST_STATIC_CONSTANT( unsigned int, translated_row = sub_matrix::template translate_row<Row>::value );
            BOOST_STATIC_CONSTANT( unsigned int, translated_col = sub_matrix::template translate_col<Column>::value );
                        
            typedef typename geometrix::type_at
                <
                    typename sub_matrix::item_type
                  , translated_row
                  , translated_col
                >::type type;
        };

        template <typename Matrix>
        composite_matrix& operator =( const Matrix& m )
        {
            BOOST_CONCEPT_ASSERT(( CompileTimeAccessConcept<M> ));
            detail::matrix_assigner<Rows,Columns>( *this, m );
            return *this;
        }

        template <unsigned int Row, unsigned int Column>
        typename type_at<Row,Column>::type get() const
        {
            return geometrix::get
                <
                    type_at<Row,Column>::translated_row
                  , type_at<Row,Column>::translated_col
                >( boost::unwrap_ref( boost::fusion::at_c<type_at<Row,Column>::index>(sequence) ) );
        }

        template <unsigned int Row, unsigned int Column>
        void set( const typename type_at<Row, Column>::type& v ) 
        {
            geometrix::set
                <
                    type_at<Row,Column>::translated_row
                  , type_at<Row,Column>::translated_col
                >( boost::unwrap_ref( boost::fusion::at_c<type_at<Row,Column>::index>(sequence) ), v );
        }

    private:
        
        boost::fusion::vector< BOOST_PP_ENUM(N, GEOMETRIX_REF_WRAPPER, A) > sequence;    
    };
        
    template <unsigned int Rows, unsigned int Columns, BOOST_PP_ENUM_PARAMS(N, typename A)>                  
    composite_matrix<boost::mpl::vector<BOOST_PP_ENUM_PARAMS(N,A)>,Rows, Columns> as_matrix(BOOST_PP_ENUM_BINARY_PARAMS(N, A, &a))                                                    
    {                                                                                                        
        return composite_matrix<boost::mpl::vector<BOOST_PP_ENUM_PARAMS(N,A)>,Rows,Columns>(BOOST_PP_ENUM_PARAMS(N,a)); 
    }
    
#undef N
#undef GEOMETRIX_REF
#undef GEOMETRIX_REF_WRAPPER
#undef GEOMETRIX_NARY_IS_HOMOGENEOUS_

#endif
