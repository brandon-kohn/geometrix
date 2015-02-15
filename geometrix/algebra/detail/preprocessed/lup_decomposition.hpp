    //
    //! Copyright © 2008-2011
    //! Brandon Kohn
    //
    //  Distributed under the Boost Software License, Version 1.0. (See
    //  accompanying file LICENSE_1_0.txt or copy at
    //  http://www.boost.org/LICENSE_1_0.txt)
    //
    
  
namespace geometrix {
    template <typename T>
    void lup_decomposition( matrix<T, 2, 2>& lu, boost::array<std::size_t, 2>& pi )
    {
        
        pi[0] = 0; pi[1] = 1;
        
        { T p = 0; std::size_t k = 0; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[0][k]) > p ) { p = math::abs(lu[0][k]); k_ = 0; } if( math::abs(lu[1][k]) > p ) { p = math::abs(lu[1][k]); k_ = 1; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); i = 1; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][1] -= lu[i][k] *lu[k][1]; } { T p = 0; std::size_t k = 1; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[1][k]) > p ) { p = math::abs(lu[1][k]); k_ = 1; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); }
        
        
    }
    template <typename T>
    boost::array<T, 2> lup_solve( matrix<T, 2, 2>& lu, const boost::array<std::size_t,2>& pi, const boost::array<T,2>& b )
    {
        boost::array<T, 2> x, y;
        T sum;
        
        { sum = 0; y[0] = b[pi[0]] - sum; } { sum = 0; sum += lu[1][0] * y[0]; y[1] = b[pi[1]] - sum; }
        
        
        
        
        { sum = 0; x[1] = ( y[1] - sum ) / lu[1] [1]; } { sum = 0; sum += lu[0][1] * x[1]; x[0] = ( y[0] - sum ) / lu[0] [0]; }
        
        return x;
    }
    
}
    
  
namespace geometrix {
    template <typename T>
    void lup_decomposition( matrix<T, 3, 3>& lu, boost::array<std::size_t, 3>& pi )
    {
        
        pi[0] = 0; pi[1] = 1; pi[2] = 2;
        
        { T p = 0; std::size_t k = 0; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[0][k]) > p ) { p = math::abs(lu[0][k]); k_ = 0; } if( math::abs(lu[1][k]) > p ) { p = math::abs(lu[1][k]); k_ = 1; } if( math::abs(lu[2][k]) > p ) { p = math::abs(lu[2][k]); k_ = 2; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); std::swap(lu[k][2] , lu[k_][2] ); i = 1; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][1] -= lu[i][k] *lu[k][1]; lu[i][2] -= lu[i][k] *lu[k][2]; i = 2; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][1] -= lu[i][k] *lu[k][1]; lu[i][2] -= lu[i][k] *lu[k][2]; } { T p = 0; std::size_t k = 1; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[1][k]) > p ) { p = math::abs(lu[1][k]); k_ = 1; } if( math::abs(lu[2][k]) > p ) { p = math::abs(lu[2][k]); k_ = 2; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); std::swap(lu[k][2] , lu[k_][2] ); i = 2; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][2] -= lu[i][k] *lu[k][2]; } { T p = 0; std::size_t k = 2; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[2][k]) > p ) { p = math::abs(lu[2][k]); k_ = 2; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); std::swap(lu[k][2] , lu[k_][2] ); }
        
        
    }
    template <typename T>
    boost::array<T, 3> lup_solve( matrix<T, 3, 3>& lu, const boost::array<std::size_t,3>& pi, const boost::array<T,3>& b )
    {
        boost::array<T, 3> x, y;
        T sum;
        
        { sum = 0; y[0] = b[pi[0]] - sum; } { sum = 0; sum += lu[1][0] * y[0]; y[1] = b[pi[1]] - sum; } { sum = 0; sum += lu[2][0] * y[0]; sum += lu[2][1] * y[1]; y[2] = b[pi[2]] - sum; }
        
        
        
        
        { sum = 0; x[2] = ( y[2] - sum ) / lu[2] [2]; } { sum = 0; sum += lu[1][2] * x[2]; x[1] = ( y[1] - sum ) / lu[1] [1]; } { sum = 0; sum += lu[0][1] * x[1]; sum += lu[0][2] * x[2]; x[0] = ( y[0] - sum ) / lu[0] [0]; }
        
        return x;
    }
    
}
    
  
namespace geometrix {
    template <typename T>
    void lup_decomposition( matrix<T, 4, 4>& lu, boost::array<std::size_t, 4>& pi )
    {
        
        pi[0] = 0; pi[1] = 1; pi[2] = 2; pi[3] = 3;
        
        { T p = 0; std::size_t k = 0; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[0][k]) > p ) { p = math::abs(lu[0][k]); k_ = 0; } if( math::abs(lu[1][k]) > p ) { p = math::abs(lu[1][k]); k_ = 1; } if( math::abs(lu[2][k]) > p ) { p = math::abs(lu[2][k]); k_ = 2; } if( math::abs(lu[3][k]) > p ) { p = math::abs(lu[3][k]); k_ = 3; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); std::swap(lu[k][2] , lu[k_][2] ); std::swap(lu[k][3] , lu[k_][3] ); i = 1; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][1] -= lu[i][k] *lu[k][1]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; i = 2; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][1] -= lu[i][k] *lu[k][1]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; i = 3; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][1] -= lu[i][k] *lu[k][1]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; } { T p = 0; std::size_t k = 1; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[1][k]) > p ) { p = math::abs(lu[1][k]); k_ = 1; } if( math::abs(lu[2][k]) > p ) { p = math::abs(lu[2][k]); k_ = 2; } if( math::abs(lu[3][k]) > p ) { p = math::abs(lu[3][k]); k_ = 3; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); std::swap(lu[k][2] , lu[k_][2] ); std::swap(lu[k][3] , lu[k_][3] ); i = 2; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; i = 3; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; } { T p = 0; std::size_t k = 2; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[2][k]) > p ) { p = math::abs(lu[2][k]); k_ = 2; } if( math::abs(lu[3][k]) > p ) { p = math::abs(lu[3][k]); k_ = 3; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); std::swap(lu[k][2] , lu[k_][2] ); std::swap(lu[k][3] , lu[k_][3] ); i = 3; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][3] -= lu[i][k] *lu[k][3]; } { T p = 0; std::size_t k = 3; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[3][k]) > p ) { p = math::abs(lu[3][k]); k_ = 3; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); std::swap(lu[k][2] , lu[k_][2] ); std::swap(lu[k][3] , lu[k_][3] ); }
        
        
    }
    template <typename T>
    boost::array<T, 4> lup_solve( matrix<T, 4, 4>& lu, const boost::array<std::size_t,4>& pi, const boost::array<T,4>& b )
    {
        boost::array<T, 4> x, y;
        T sum;
        
        { sum = 0; y[0] = b[pi[0]] - sum; } { sum = 0; sum += lu[1][0] * y[0]; y[1] = b[pi[1]] - sum; } { sum = 0; sum += lu[2][0] * y[0]; sum += lu[2][1] * y[1]; y[2] = b[pi[2]] - sum; } { sum = 0; sum += lu[3][0] * y[0]; sum += lu[3][1] * y[1]; sum += lu[3][2] * y[2]; y[3] = b[pi[3]] - sum; }
        
        
        
        
        { sum = 0; x[3] = ( y[3] - sum ) / lu[3] [3]; } { sum = 0; sum += lu[2][3] * x[3]; x[2] = ( y[2] - sum ) / lu[2] [2]; } { sum = 0; sum += lu[1][2] * x[2]; sum += lu[1][3] * x[3]; x[1] = ( y[1] - sum ) / lu[1] [1]; } { sum = 0; sum += lu[0][1] * x[1]; sum += lu[0][2] * x[2]; sum += lu[0][3] * x[3]; x[0] = ( y[0] - sum ) / lu[0] [0]; }
        
        return x;
    }
    
}
    
  
namespace geometrix {
    template <typename T>
    void lup_decomposition( matrix<T, 5, 5>& lu, boost::array<std::size_t, 5>& pi )
    {
        
        pi[0] = 0; pi[1] = 1; pi[2] = 2; pi[3] = 3; pi[4] = 4;
        
        {
            T p = 0; std::size_t k = 0; std::size_t k_ = 0; 
            std::size_t i = 0; 
            if( math::abs(lu[0][k]) > p ) { p = math::abs(lu[0][k]); k_ = 0; } 
            if( math::abs(lu[1][k]) > p ) { p = math::abs(lu[1][k]); k_ = 1; } 
            if( math::abs(lu[2][k]) > p ) { p = math::abs(lu[2][k]); k_ = 2; } 
            if( math::abs(lu[3][k]) > p ) { p = math::abs(lu[3][k]); k_ = 3; } 
            if( math::abs(lu[4][k]) > p ) { p = math::abs(lu[4][k]); k_ = 4; } 
            if( p == 0 ) 
                throw std::logic_error( "cannot lup decompose a singular matrix." ); 
            std::swap( pi[k], pi[k_] );
            std::swap(lu[k][0] , lu[k_][0] );
            std::swap(lu[k][1] , lu[k_][1] );
            std::swap(lu[k][2] , lu[k_][2] );
            std::swap(lu[k][3] , lu[k_][3] );
            std::swap(lu[k][4] , lu[k_][4] );
            i = 1; 
            lu[i][k] = lu[i][k]/lu[k][k];
            lu[i][1] -= lu[i][k] *lu[k][1];
            lu[i][2] -= lu[i][k] *lu[k][2];
            lu[i][3] -= lu[i][k] *lu[k][3];
            lu[i][4] -= lu[i][k] *lu[k][4];
            i = 2; lu[i][k] = lu[i][k]/lu[k][k]; 
            lu[i][1] -= lu[i][k] *lu[k][1]; 
            lu[i][2] -= lu[i][k] *lu[k][2];
            lu[i][3] -= lu[i][k] *lu[k][3];
            lu[i][4] -= lu[i][k] *lu[k][4];
            i = 3; 
            lu[i][k] = lu[i][k]/lu[k][k];
            lu[i][1] -= lu[i][k] *lu[k][1];
            lu[i][2] -= lu[i][k] *lu[k][2];
            lu[i][3] -= lu[i][k] *lu[k][3];
            lu[i][4] -= lu[i][k] *lu[k][4];
            i = 4;
            lu[i][k] = lu[i][k]/lu[k][k];
            lu[i][1] -= lu[i][k] *lu[k][1]; 
            lu[i][2] -= lu[i][k] *lu[k][2];
            lu[i][3] -= lu[i][k] *lu[k][3]; 
            lu[i][4] -= lu[i][k] *lu[k][4];
        } 
        {
            T p = 0; std::size_t k = 1; std::size_t k_ = 0; std::size_t i = 0;
            if( math::abs(lu[1][k]) > p ) { p = math::abs(lu[1][k]); k_ = 1; } 
            if( math::abs(lu[2][k]) > p ) { p = math::abs(lu[2][k]); k_ = 2; } 
            if( math::abs(lu[3][k]) > p ) { p = math::abs(lu[3][k]); k_ = 3; } 
            if( math::abs(lu[4][k]) > p ) { p = math::abs(lu[4][k]); k_ = 4; }
            if( p == 0 )
                throw std::logic_error( "cannot lup decompose a singular matrix." );
            std::swap( pi[k], pi[k_] );
            std::swap(lu[k][0] , lu[k_][0] );
            std::swap(lu[k][1] , lu[k_][1] );
            std::swap(lu[k][2] , lu[k_][2] );
            std::swap(lu[k][3] , lu[k_][3] );
            std::swap(lu[k][4] , lu[k_][4] );
            i = 2;
            lu[i][k] = lu[i][k]/lu[k][k];
            lu[i][2] -= lu[i][k] *lu[k][2];
            lu[i][3] -= lu[i][k] *lu[k][3];
            lu[i][4] -= lu[i][k] *lu[k][4]; i = 3;
            lu[i][k] = lu[i][k]/lu[k][k];
            lu[i][2] -= lu[i][k] *lu[k][2];
            lu[i][3] -= lu[i][k] *lu[k][3];
            lu[i][4] -= lu[i][k] *lu[k][4]; 
            i = 4; lu[i][k] = lu[i][k]/lu[k][k];
            lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; lu[i][4] -= lu[i][k] *lu[k][4]; 
        }
        {
            T p = 0; std::size_t k = 2; std::size_t k_ = 0; std::size_t i = 0;
            if( math::abs(lu[2][k]) > p ) { p = math::abs(lu[2][k]); k_ = 2; } 
            if( math::abs(lu[3][k]) > p ) { p = math::abs(lu[3][k]); k_ = 3; } 
            if( math::abs(lu[4][k]) > p ) { p = math::abs(lu[4][k]); k_ = 4; } 
            if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); 
            std::swap( pi[k], pi[k_] );
            std::swap(lu[k][0] , lu[k_][0] );
            std::swap(lu[k][1] , lu[k_][1] );
            std::swap(lu[k][2] , lu[k_][2] );
            std::swap(lu[k][3] , lu[k_][3] );
            std::swap(lu[k][4] , lu[k_][4] );
            i = 3; lu[i][k] = lu[i][k]/lu[k][k];
            lu[i][3] -= lu[i][k] *lu[k][3];
            lu[i][4] -= lu[i][k] *lu[k][4];
            i = 4; lu[i][k] = lu[i][k]/lu[k][k];
            lu[i][3] -= lu[i][k] *lu[k][3];
            lu[i][4] -= lu[i][k] *lu[k][4]; } 
        {
            T p = 0; std::size_t k = 3; std::size_t k_ = 0; std::size_t i = 0;
            if( math::abs(lu[3][k]) > p ) { p = math::abs(lu[3][k]); k_ = 3; }
            if( math::abs(lu[4][k]) > p ) { p = math::abs(lu[4][k]); k_ = 4; }
            if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); 
            std::swap( pi[k], pi[k_] );
            std::swap(lu[k][0] , lu[k_][0] );
            std::swap(lu[k][1] , lu[k_][1] );
            std::swap(lu[k][2] , lu[k_][2] );
            std::swap(lu[k][3] , lu[k_][3] );
            std::swap(lu[k][4] , lu[k_][4] );
            i = 4;
            lu[i][k] = lu[i][k]/lu[k][k];
            lu[i][4] -= lu[i][k] *lu[k][4];
        } 
        { 
            T p = 0; std::size_t k = 4; std::size_t k_ = 0;
            if( math::abs(lu[4][k]) > p ) { p = math::abs(lu[4][k]); k_ = 4; }
            if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); 
            std::swap( pi[k], pi[k_] ); 
            std::swap(lu[k][0] , lu[k_][0] );
            std::swap(lu[k][1] , lu[k_][1] );
            std::swap(lu[k][2] , lu[k_][2] );
            std::swap(lu[k][3] , lu[k_][3] );
            std::swap(lu[k][4] , lu[k_][4] );
        }


    }
    template <typename T>
    boost::array<T, 5> lup_solve( matrix<T, 5, 5>& lu, const boost::array<std::size_t,5>& pi, const boost::array<T,5>& b )
    {
        boost::array<T, 5> x, y;
        T sum;
        
        { sum = 0; y[0] = b[pi[0]] - sum; } { sum = 0; sum += lu[1][0] * y[0]; y[1] = b[pi[1]] - sum; } { sum = 0; sum += lu[2][0] * y[0]; sum += lu[2][1] * y[1]; y[2] = b[pi[2]] - sum; } { sum = 0; sum += lu[3][0] * y[0]; sum += lu[3][1] * y[1]; sum += lu[3][2] * y[2]; y[3] = b[pi[3]] - sum; } { sum = 0; sum += lu[4][0] * y[0]; sum += lu[4][1] * y[1]; sum += lu[4][2] * y[2]; sum += lu[4][3] * y[3]; y[4] = b[pi[4]] - sum; }
        { sum = 0; x[4] = ( y[4] - sum ) / lu[4] [4]; } { sum = 0; sum += lu[3][4] * x[4]; x[3] = ( y[3] - sum ) / lu[3] [3]; } { sum = 0; sum += lu[2][3] * x[3]; sum += lu[2][4] * x[4]; x[2] = ( y[2] - sum ) / lu[2] [2]; } { sum = 0; sum += lu[1][2] * x[2]; sum += lu[1][3] * x[3]; sum += lu[1][4] * x[4]; x[1] = ( y[1] - sum ) / lu[1] [1]; } { sum = 0; sum += lu[0][1] * x[1]; sum += lu[0][2] * x[2]; sum += lu[0][3] * x[3]; sum += lu[0][4] * x[4]; x[0] = ( y[0] - sum ) / lu[0] [0]; }
        
        return x;
    }
    
}
    
  
namespace geometrix {
    template <typename T>
    void lup_decomposition( matrix<T, 6, 6>& lu, boost::array<std::size_t, 6>& pi )
    {
        
        pi[0] = 0; pi[1] = 1; pi[2] = 2; pi[3] = 3; pi[4] = 4; pi[5] = 5;
        
        { T p = 0; std::size_t k = 0; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[0][k]) > p ) { p = math::abs(lu[0][k]); k_ = 0; } if( math::abs(lu[1][k]) > p ) { p = math::abs(lu[1][k]); k_ = 1; } if( math::abs(lu[2][k]) > p ) { p = math::abs(lu[2][k]); k_ = 2; } if( math::abs(lu[3][k]) > p ) { p = math::abs(lu[3][k]); k_ = 3; } if( math::abs(lu[4][k]) > p ) { p = math::abs(lu[4][k]); k_ = 4; } if( math::abs(lu[5][k]) > p ) { p = math::abs(lu[5][k]); k_ = 5; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); std::swap(lu[k][2] , lu[k_][2] ); std::swap(lu[k][3] , lu[k_][3] ); std::swap(lu[k][4] , lu[k_][4] ); std::swap(lu[k][5] , lu[k_][5] ); i = 1; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][1] -= lu[i][k] *lu[k][1]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; i = 2; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][1] -= lu[i][k] *lu[k][1]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; i = 3; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][1] -= lu[i][k] *lu[k][1]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; i = 4; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][1] -= lu[i][k] *lu[k][1]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; i = 5; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][1] -= lu[i][k] *lu[k][1]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; } { T p = 0; std::size_t k = 1; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[1][k]) > p ) { p = math::abs(lu[1][k]); k_ = 1; } if( math::abs(lu[2][k]) > p ) { p = math::abs(lu[2][k]); k_ = 2; } if( math::abs(lu[3][k]) > p ) { p = math::abs(lu[3][k]); k_ = 3; } if( math::abs(lu[4][k]) > p ) { p = math::abs(lu[4][k]); k_ = 4; } if( math::abs(lu[5][k]) > p ) { p = math::abs(lu[5][k]); k_ = 5; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); std::swap(lu[k][2] , lu[k_][2] ); std::swap(lu[k][3] , lu[k_][3] ); std::swap(lu[k][4] , lu[k_][4] ); std::swap(lu[k][5] , lu[k_][5] ); i = 2; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; i = 3; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; i = 4; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; i = 5; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][2] -= lu[i][k] *lu[k][2]; lu[i][3] -= lu[i][k] *lu[k][3]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; } { T p = 0; std::size_t k = 2; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[2][k]) > p ) { p = math::abs(lu[2][k]); k_ = 2; } if( math::abs(lu[3][k]) > p ) { p = math::abs(lu[3][k]); k_ = 3; } if( math::abs(lu[4][k]) > p ) { p = math::abs(lu[4][k]); k_ = 4; } if( math::abs(lu[5][k]) > p ) { p = math::abs(lu[5][k]); k_ = 5; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); std::swap(lu[k][2] , lu[k_][2] ); std::swap(lu[k][3] , lu[k_][3] ); std::swap(lu[k][4] , lu[k_][4] ); std::swap(lu[k][5] , lu[k_][5] ); i = 3; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][3] -= lu[i][k] *lu[k][3]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; i = 4; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][3] -= lu[i][k] *lu[k][3]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; i = 5; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][3] -= lu[i][k] *lu[k][3]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; } { T p = 0; std::size_t k = 3; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[3][k]) > p ) { p = math::abs(lu[3][k]); k_ = 3; } if( math::abs(lu[4][k]) > p ) { p = math::abs(lu[4][k]); k_ = 4; } if( math::abs(lu[5][k]) > p ) { p = math::abs(lu[5][k]); k_ = 5; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); std::swap(lu[k][2] , lu[k_][2] ); std::swap(lu[k][3] , lu[k_][3] ); std::swap(lu[k][4] , lu[k_][4] ); std::swap(lu[k][5] , lu[k_][5] ); i = 4; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; i = 5; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][4] -= lu[i][k] *lu[k][4]; lu[i][5] -= lu[i][k] *lu[k][5]; } { T p = 0; std::size_t k = 4; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[4][k]) > p ) { p = math::abs(lu[4][k]); k_ = 4; } if( math::abs(lu[5][k]) > p ) { p = math::abs(lu[5][k]); k_ = 5; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); std::swap(lu[k][2] , lu[k_][2] ); std::swap(lu[k][3] , lu[k_][3] ); std::swap(lu[k][4] , lu[k_][4] ); std::swap(lu[k][5] , lu[k_][5] ); i = 5; lu[i][k] = lu[i][k]/lu[k][k]; lu[i][5] -= lu[i][k] *lu[k][5]; } { T p = 0; std::size_t k = 5; std::size_t k_ = 0; std::size_t i = 0; if( math::abs(lu[5][k]) > p ) { p = math::abs(lu[5][k]); k_ = 5; } if( p == 0 ) throw std::logic_error( "cannot lup decompose a singular matrix." ); std::swap( pi[k], pi[k_] ); std::swap(lu[k][0] , lu[k_][0] ); std::swap(lu[k][1] , lu[k_][1] ); std::swap(lu[k][2] , lu[k_][2] ); std::swap(lu[k][3] , lu[k_][3] ); std::swap(lu[k][4] , lu[k_][4] ); std::swap(lu[k][5] , lu[k_][5] ); }
        
        
    }
    template <typename T>
    boost::array<T, 6> lup_solve( matrix<T, 6, 6>& lu, const boost::array<std::size_t,6>& pi, const boost::array<T,6>& b )
    {
        boost::array<T, 6> x, y;
        T sum;
        
        { sum = 0; y[0] = b[pi[0]] - sum; } { sum = 0; sum += lu[1][0] * y[0]; y[1] = b[pi[1]] - sum; } { sum = 0; sum += lu[2][0] * y[0]; sum += lu[2][1] * y[1]; y[2] = b[pi[2]] - sum; } { sum = 0; sum += lu[3][0] * y[0]; sum += lu[3][1] * y[1]; sum += lu[3][2] * y[2]; y[3] = b[pi[3]] - sum; } { sum = 0; sum += lu[4][0] * y[0]; sum += lu[4][1] * y[1]; sum += lu[4][2] * y[2]; sum += lu[4][3] * y[3]; y[4] = b[pi[4]] - sum; } { sum = 0; sum += lu[5][0] * y[0]; sum += lu[5][1] * y[1]; sum += lu[5][2] * y[2]; sum += lu[5][3] * y[3]; sum += lu[5][4] * y[4]; y[5] = b[pi[5]] - sum; }
        { sum = 0; x[5] = ( y[5] - sum ) / lu[5] [5]; } { sum = 0; sum += lu[4][5] * x[5]; x[4] = ( y[4] - sum ) / lu[4] [4]; } { sum = 0; sum += lu[3][4] * x[4]; sum += lu[3][5] * x[5]; x[3] = ( y[3] - sum ) / lu[3] [3]; } { sum = 0; sum += lu[2][3] * x[3]; sum += lu[2][4] * x[4]; sum += lu[2][5] * x[5]; x[2] = ( y[2] - sum ) / lu[2] [2]; } { sum = 0; sum += lu[1][2] * x[2]; sum += lu[1][3] * x[3]; sum += lu[1][4] * x[4]; sum += lu[1][5] * x[5]; x[1] = ( y[1] - sum ) / lu[1] [1]; } { sum = 0; sum += lu[0][1] * x[1]; sum += lu[0][2] * x[2]; sum += lu[0][3] * x[3]; sum += lu[0][4] * x[4]; sum += lu[0][5] * x[5]; x[0] = ( y[0] - sum ) / lu[0] [0]; }
        
        return x;
    }
    
}
