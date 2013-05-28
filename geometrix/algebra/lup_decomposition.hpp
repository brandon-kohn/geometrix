//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_LUP_DECOMPOSITION_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_LUP_DECOMPOSITION_HPP

#include <geometrix/utility/preprocessor.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <exception>

#if !defined( GEOMETRIX_USE_RUN_TIME_LUP_DECOMPOSITION )

    #if !defined( GEOMETRIX_MAX_LUP_DECOMPOSITION_MATRIX_SIZE )
        #define GEOMETRIX_MAX_LUP_DECOMPOSITION_MATRIX_SIZE 6
    #endif

    #include <geometrix/algebra/detail/lup_decomposition.hpp>

#else
namespace geometrix { namespace algebra {

    template <typename T, unsigned int N>
    void lup_decomposition( matrix<T, N, N>& m, boost::array<std::size_t, N>& pi )
    {
        //! Initialize PI as zeros.
        for( std::size_t i = 0; i < N; ++i )
            pi[i] = i;
        for( std::size_t k = 0; k < N - 1; ++k )
        {
            T p = 0;
            std::size_t k_ = 0;
            for( std::size_t i = k; i < N; ++i )
            {
                if( abs(m[i][k]) > p )
                {
                    p = abs(m[i][k]);
                    k_ = i;
                }
            }

            if( p == 0 )
                throw std::logic_error( "cannot lup decompose a singular matrix." );
            std::swap( pi[k], pi[k_] );
            for( std::size_t i = 0; i < N; ++i )
                std::swap( m[k][i], m[k_][i] );
            for( std::size_t i = k + 1; i < N; ++i )
            {
                m[i][k] /= m[k][k];
                for( std::size_t j = k + 1; j < N; ++j )
                    m[i][j] -= m[i][k]*m[k][j];
            }
        }
    }

    template <typename T>
    boost::array<T, N> lup_solve( matrix<T, N, N>& lu, const boost::array<std::size_t,N>& pi, const boost::array<T,N>& b )
    {
        boost::array<T, N> x, y;
        for( std::size_t i = 0; i < N; ++i )
        {
            T sum = 0;
            for( std::size_t j = 0; j < i; ++j )
                sum += lu[i][j] * y[j];
            y[i] = b[pi[i]] - sum;
        }
        for( std::size_t i = N-1; i >= 0; --i )
        {
            T sum = 0;
            for( std::size_t j = i+1; j < N; ++j )
                sum += lu[i][j] * x[j];
            x[i] = ( y[i] - sum ) / lu[i][i];

            if( i == 0 )
                break;
        }

        return x;
    }
}}//namespace geometrix::algebra

#endif//GEOMETRIX_USE_RUN_TIME_DECOMPOSITION
#endif //GEOMETRIX_LINEAR_ALGEBRA_LUP_DECOMPOSITION_HPP
