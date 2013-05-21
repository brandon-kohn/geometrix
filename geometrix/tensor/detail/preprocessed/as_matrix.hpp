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
                typedef interval<RowOffset, RowOffset+geometrix::row_dimension_of<Item>::value-1> row_interval;
                typedef interval<ColumnOffset, ColumnOffset+geometrix::column_dimension_of<Item>::value-1> column_interval;
                typedef Item item_type;
                typedef Index index;
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
                  , unsigned int NextRow = RowIndex + 10//! Set this to an effective infinitiy by default (new rows)
                >
            struct state
            {
                typedef boost::mpl::int_<RowIndex> row_index; //! The current row index
typedef boost::mpl::int_<ColIndex> col_index; //! The current col index
typedef boost::mpl::int_<NextRow> next_row; //! The min index for the next row.
typedef RowIter row_iter; //! The iterator in the last complete row.
typedef Row row; //! The current row.
typedef LastRow last_row; //! The previous row.
typedef boost::mpl::int_<Index> index; //! The index of the next item in the composition sequence.
typedef Sequence sequence;
                typedef TransformedSequence result;
            };
            template 
                <
                    typename Sequence 
                  , typename TransformedSequence = boost::mpl::vector<>
                  , unsigned int ColIndex = 0 
                  , typename Row = boost::mpl::vector<> 
                  , unsigned int Index = 0
                  , unsigned int NextRow = 10//! Set this to an effective infinitiy by default (new rows)
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
            
        };
        template 
            <
                unsigned int Rows
              , unsigned int Columns
              , BOOST_PP_ENUM
                (
                    10
                  , GEOMETRIX_NARY_ARGS_ENABLE_IF_
                  , _
                )
              , typename EnableIf=void
            >
        struct as_matrix{};
    }//namespace result_of;
    
    template 
        <
            typename Sequence
          , BOOST_PP_ENUM
            (
                10
              , GEOMETRIX_NARY_HOMOGENEOUS_ENABLE_IF_
              , _
            )
        >
    struct composite_diversity_base
    {
        typedef void is_heterogeneous;
    };
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
        typedef dimension<Rows> row_dimension;
        typedef dimension<Columns> col_dimension;
        typedef void is_matrix;
    };
    template <typename Matrix>
    struct composite_matrix_access_policy
    {
        typedef void compile_time_access;
        template <unsigned int Row, unsigned int Column>
        struct type_at
            : Matrix::template type_at<Row,Column>
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
        typedef boost::mpl::int_<2> tensor_order;
        typedef void rank_2;
        typedef void is_tensor;
    };
    template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Column>
    struct column< composite_matrix<Sequence, Rows, Columns>, Column > 
    { 
        typedef boost::mpl::int_<Column> index;
        typedef composite_matrix<Sequence,Rows, Columns> matrix_type;
    
        explicit column( const matrix_type& m )
            : m(m)
        {}
        template <unsigned int Row>
        struct type_at
            : matrix_type::template type_at<Row,index::value>
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
        typedef void is_sequence;
        typedef void is_numeric_sequence;
        typedef void is_vector; 
    };
    template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Column>
    struct tensor_traits< column<composite_matrix<Sequence, Rows, Columns>, Column> >
    {
        typedef matrix_slice_access_policy< column<composite_matrix<Sequence,Rows,Columns>, Column> > access_policy;
        typedef boost::mpl::int_<1> tensor_order;
        typedef void rank_1;
        typedef void is_tensor;
        typedef void make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
    };
    template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Row>
    struct row< composite_matrix<Sequence, Rows, Columns>, Row > 
    { 
        typedef boost::mpl::int_<Row> index;
        typedef composite_matrix<Sequence, Rows, Columns> matrix_type;
    
        explicit row( const matrix_type& m )
            : m(m)
        {}
        template <unsigned int Column>
        struct type_at
            : matrix_type::template type_at<index::value,Column>
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
        typedef void is_sequence;
        typedef void is_numeric_sequence;
        typedef void is_vector; 
    };
    template <typename Sequence, unsigned int Rows, unsigned int Columns, unsigned int Row>
    struct tensor_traits< row<composite_matrix<Sequence, Rows, Columns>, Row> >
    {
        typedef matrix_slice_access_policy< row<composite_matrix<Sequence,Rows,Columns>,Row> > access_policy;
        typedef boost::mpl::int_<1> tensor_order;
        typedef void rank_1;
        typedef void is_tensor;
        typedef void make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
    };
    
        
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
