//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ARITHMETIC_MATRIX_TRACE_HPP
#define GEOMETRIX_ARITHMETIC_MATRIX_TRACE_HPP

#include <geometrix/tensor/matrix_traits.hpp>
#include <boost/concept/assert.hpp>

namespace geometrix {
    
    namespace detail{
        
        template<std::size_t I, typename Sum, typename Matrix>
        struct trace_sum
        {
        private:
            using next_t = trace_sum<I-1, decltype(std::declval<typename type_at<Matrix, I-1, I-1>::type>() + std::declval<Sum>()), Matrix>;
        public:
        
            using type = typename next_t::type;
        
            static type apply(const Matrix& m, const Sum& s)
            {
                return next_t::apply(m, s + get<I-1,I-1>(m));
            }
        };
        
        template<typename Sum, typename Matrix>
        struct trace_sum<0, Sum, Matrix>
        {
            using type = decltype(std::declval<typename type_at<Matrix, 0, 0>::type>() + std::declval<Sum>());
            
            static type apply(const Matrix& m, const Sum& s)
            {
                return s;
            }
        };
    }//! namespace detail;

    //! Trace of a Matrix - Sum of the diagonals.
    namespace result_of {
        
        template <typename Matrix>
        struct trace 
            : ::geometrix::detail::trace_sum
              <
                geometric_traits<Matrix>::row_dimension::value - 2
              , typename type_at<Matrix, geometric_traits<Matrix>::row_dimension::value - 1, geometric_traits<Matrix>::row_dimension::value - 1>::type
              , Matrix
              >
        {};
        
    }//! namespace result_of;
    
    template <typename Matrix>
    inline typename result_of::trace<Matrix>::type trace(const Matrix& m)
    {
        BOOST_CONCEPT_ASSERT(( SquareMatrixConcept<Matrix> ));        
        auto v = get<geometric_traits<Matrix>::row_dimension::value - 1,geometric_traits<Matrix>::row_dimension::value - 1>(m);
        return detail::trace_sum<geometric_traits<Matrix>::row_dimension::value - 1, decltype(v), Matrix>::apply(m, v);
    }
    
}//namespace geometrix;

#endif//GEOMETRIX_ARITHMETIC_MATRIX_TRACE_HPP
