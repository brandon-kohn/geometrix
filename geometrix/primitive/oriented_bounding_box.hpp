//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ORIENTED_BOUNDING_BOX_HPP
#define GEOMETRIX_ORIENTED_BOUNDING_BOX_HPP

#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/primitive/rectangle.hpp>

#include <stdexcept>
#include <type_traits>
#include <array>

namespace geometrix {

    template <typename Point, typename Vector, typename EnableIf=void>
    class oriented_bounding_box;

    //! \class oriented_bounding_box
    //! \brief A class for specifying a oriented_bounding_box with two vectors in 2 dimensions.
    template <typename Point, typename Vector>
    class oriented_bounding_box<Point, Vector, typename std::enable_if<dimension_of<Point>::value==2>::type>
    {
        BOOST_CLASS_REQUIRE( Vector, geometrix, VectorConcept );
        BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );

    public:

        typedef Vector                                                 vector_type;
        typedef Point                                                  point_type;
        typedef typename dimension_of< vector_type >::type             dimension_type;
        typedef typename geometric_traits<point_type>::arithmetic_type length_type;

        oriented_bounding_box( const point_type& a, const vector_type& u, const vector_type& v, const length_type& uHalfWidth, const length_type& vHalfWidth )
            : m_center(a)
#if BOOST_WORKAROUND(BOOST_MSVC, < 1900)
            , m_axes({u, v})
            , m_halfWidths({uHalfWidth, vHalfWidth})
#else
            , m_axes{u, v}
            , m_halfWidths{uHalfWidth, vHalfWidth}
#endif
        {
            static_assert(std::is_same<typename geometric_traits<vector_type>::arithmetic_type, typename geometric_traits<vector_type>::dimensionless_type>::value, "oriented_bounding_box vectors should be unit and dimensionless.");
        }

        ~oriented_bounding_box(){}

        const point_type&  get_center() const { return m_center; }

        template <typename PointExpr>
        void set_center(const PointExpr& c){ assign(m_center, c); }

        BOOST_CONSTEXPR const vector_type& get_axis(std::size_t Index) const
        {
            return m_axes[Index];
        }

        BOOST_CONSTEXPR const length_type& get_halfwidth(std::size_t Index) const
        {
            return m_halfWidths[Index];
        }

        std::size_t size() const { return 4; }

        //! Convenience border point accessor.
        point_type operator[](std::size_t i) const
        {
            GEOMETRIX_ASSERT(i < 4);
            switch (i)
            {
            case 0:
                return get_right_backward_point();
            case 1:
                return get_right_forward_point();
            case 2:
                return get_left_forward_point();
            case 3:
                return get_left_backward_point();
            };

            throw std::out_of_range("index out of range; box has 4 points");
        }

        //! For use in Polygonal operations, this is more efficient when accessing vertices multiple times.
        rectangle<point_type> get_rectangle() const { return rectangle<point_type>{ (*this)[0], (*this)[1], (*this)[2], (*this)[3] }; }

    private:

        point_type get_left_forward_point() const { return m_center + m_halfWidths[0] * m_axes[0] + m_halfWidths[1] * m_axes[1]; }
        point_type get_right_backward_point() const { return m_center - m_halfWidths[0] * m_axes[0] - m_halfWidths[1] * m_axes[1]; }
        point_type get_right_forward_point() const { return m_center + m_halfWidths[0] * m_axes[0] - m_halfWidths[1] * m_axes[1]; }
        point_type get_left_backward_point() const { return m_center - m_halfWidths[0] * m_axes[0] + m_halfWidths[1] * m_axes[1]; }

        //! Oriented bounding box defined by a center, and two orthogonal unit vectors defining the orientation in R2.
        point_type  m_center;
        std::array<vector_type, dimension_of<vector_type>::value> m_axes;
        std::array<length_type, dimension_of<vector_type>::value> m_halfWidths;

    };

    typedef oriented_bounding_box<point_double_2d, vector_double_2d> obb_double_2d;

    //! Create an obb from a rectangle. The 0th point in the rectangle is the right backward point by convention.
    template <typename Point, typename Vector>
    inline oriented_bounding_box<Point, Vector> make_obb(const rectangle<Point>& r)
    {
        static_assert(dimension_of<Point>::value == 2, "TODO: make 3D version of obb.");
        auto uWidth = point_point_distance(r[1], r[0]);
        Vector u = construct<Vector>((r[1] - r[0]) / uWidth);
        auto vWidth = point_point_distance(r[3], r[0]);
        Vector v = construct<Vector>((r[3] - r[0]) / vWidth);
        uWidth *= 0.5;
        vWidth *= 0.5;
        return oriented_bounding_box<Point, Vector>(r[0] + uWidth * u + vWidth * v, u, v, uWidth, vWidth);
    }

}//namespace geometrix;

#endif //GEOMETRIX_ORIENTED_BOUNDING_BOX_HPP
