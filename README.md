Geometrix
=========

Geometrix is a computational geometry library developed with the primary goal of allowing developers to express geometric and algebraic constructs in the same language as they are expressed in textbooks. For example, a common geometry problem is to find the intersection point between two intersecting line segments. This can be algebraically expressed as: 

Given points A, B, C, D which define properly intersecting line segments AB, CD the intersection point I is defined as:
I = A + s *(B-A), where s is a scalar parameter whose value represents a scale factor to the vector B-A which designates where the intersection occurs.

Many geometry libraries require simple constructs like this to be made explicitly by manipulating the elements of the points and vectors involved in the computation.

e.g.: 

	I.x = A.x + s * (B.x-A.x)
	I.y = A.y + s * (B.y-A.y)

With geometrix the calculation can be expressed as:

	point<double,2> I = A + s * (A-B);

This simpler form is true to the original intention and is optimised through the use of expression templates.

Here is an example:

	//! Determine if 4 points are coplanar.
	template <typename Point1, typename Point2, typename Point3, typename Point4>
	inline bool is_coplanar( const Point1& x1, const Point2& x2, const Point3& x3, const Point4& x4 )
	{
		using namespace geometrix;
		using namespace geometrix::algebra;
		double a;
		//! * represents the dot product when applied to vectors.
		//! ^ represents the cross product when applied to vectors.
		a <<= (x3-x1)*((x2-x1)^(x4-x3));
		return a == 0; 
	}

	//! Calculate whether two lines intersect, and specify the intersection point if they do.
	template <typename Point1, typename Vector1, typename Point2, typename Vector2, typename IPoint>
	inline bool lines_intersect( const Point1& p1, const Vector1& v1, const Point2& p2, const Vector2& v2, IPoint& iPoint )
	{
		using namespace geometrix;
		using namespace geometrix::algebra;
		using namespace boost::lamda;
		
		if( !is_coplanar( p1, p1+v1, p2, p2+v2 ) )
			return false;
		
		auto u = v1^v2; 
		auto m = magnitude_sqrd(u);

		//! Lines are parallel if m==0.
		if( m == 0 )
			return boost::fusion::all( u, _1 == 0 );        
		
		iPoint <<= p1 + ( (((p2-p1)^v2) * u ) / m ) * v1;
		
		return true;
	}