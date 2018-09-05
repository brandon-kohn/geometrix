#pragma once

#include <array>
#include <limits>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/numeric/interval.hpp>
#include <geometrix/tensor/is_null.hpp>

template <typename Point>
class oriented_bounding_box_2d
{
	using point_type = Point;
	using length_type = typename geometrix::arithmetic_type_of<point_type>::type;
	using dimensionless_type = typename geometrix::dimensionless_type_of<point_type>::type;
	using dimensionless2 = geometrix::vector<dimensionless_type, 2>;
	using interval_type = geometrix::interval<length_type>;

public:
	template<typename Polygon>
	oriented_bounding_box_2d(const Polygon& polygon);
	oriented_bounding_box_2d() = default;

	template <typename NumberComparisonPolicy>
	bool intersects( const oriented_bounding_box_2d& other, const NumberComparisonPolicy& cmp ) const;
	bool empty() const
	{
		return geometrix::is_null(mUnitAxis[0]);//! unit axes won't be null vectors when the box is not default constructed.
	}

private:

	template <typename NumberComparisonPolicy>
	bool overlaps_one_way( const oriented_bounding_box_2d& other, const NumberComparisonPolicy& cmp ) const;
	void calculate_projection();

	std::array<point_type, 4>     mCorners;
	std::array<dimensionless2, 2> mUnitAxis;
	std::array<interval_type, 2>  mProjection;
};

template <typename Point>
template <typename Polygon>
inline oriented_bounding_box_2d<Point>::oriented_bounding_box_2d( const Polygon& polygon )
{
	using namespace geometrix;
	using access = point_sequence_traits<Polygon>;
	GEOMETRIX_ASSERT(access::size(polygon) == 4);

    for( int i=0; i<4; ++i )
        mCorners[i] = construct<point_type>(access::get_point(polygon,i));
    calculate_projection();
}

template <typename Point>
template <typename NumberComparisonPolicy>
inline bool oriented_bounding_box_2d<Point>::intersects(const oriented_bounding_box_2d<Point> &other, const NumberComparisonPolicy& cmp) const
{
    return this->overlaps_one_way( other, cmp ) && other.overlaps_one_way( *this, cmp );
}

template <typename Point>
template <typename NumberComparisonPolicy>
inline bool oriented_bounding_box_2d<Point>::overlaps_one_way(const oriented_bounding_box_2d<Point>& other, const NumberComparisonPolicy& cmp) const
{
	using namespace geometrix;
	{
		auto proj_int = interval_type{scalar_projection(as_vector(other.mCorners[0]), mUnitAxis[0])};
		proj_int.expand(scalar_projection(as_vector(other.mCorners[1]), mUnitAxis[0]));
		proj_int.expand(scalar_projection(as_vector(other.mCorners[2]), mUnitAxis[0]));
		proj_int.expand(scalar_projection(as_vector(other.mCorners[3]), mUnitAxis[0]));

		if (!proj_int.intersects(mProjection[0], cmp))
            return false;
    }

	{
		auto proj_int = interval_type{scalar_projection(as_vector(other.mCorners[0]), mUnitAxis[1])};
		proj_int.expand(scalar_projection(as_vector(other.mCorners[1]), mUnitAxis[1]));
		proj_int.expand(scalar_projection(as_vector(other.mCorners[2]), mUnitAxis[1]));
		proj_int.expand(scalar_projection(as_vector(other.mCorners[3]), mUnitAxis[1]));

		if (!proj_int.intersects(mProjection[1], cmp))
            return false;
    }

	return true;
}

template <typename Point>
inline void oriented_bounding_box_2d<Point>::calculate_projection(){
	using namespace geometrix;
    mUnitAxis[ 0 ] = normalize(mCorners[ 1 ] - mCorners[ 0 ]);
    mUnitAxis[ 1 ] = normalize(mCorners[ 3 ] - mCorners[ 0 ]);

	//! Build axis projection 0.
	mProjection[0].expand(scalar_projection(mCorners[0], mUnitAxis[0]));
	mProjection[0].expand(scalar_projection(mCorners[1], mUnitAxis[0]));
	mProjection[0].expand(scalar_projection(mCorners[2], mUnitAxis[0]));
	mProjection[0].expand(scalar_projection(mCorners[3], mUnitAxis[0]));

	//! Build axis projection 1.
	mProjection[1].expand(scalar_projection(mCorners[0], mUnitAxis[1]));
	mProjection[1].expand(scalar_projection(mCorners[1], mUnitAxis[1]));
	mProjection[1].expand(scalar_projection(mCorners[2], mUnitAxis[1]));
	mProjection[1].expand(scalar_projection(mCorners[3], mUnitAxis[1]));
}
