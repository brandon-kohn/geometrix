// geometry_test.cpp : Defines the entry point for the console application.
//

#include <fstream>
std::ostream& logger()
{
	static std::ofstream instance("geometry_log.hpp");
	return instance;
}

// Boost.Test
#include <boost/test/included/unit_test.hpp>

// #include "product_tests.hpp"
// 
// //#include "access_time_tests.hpp"
// //#include "linear_algebra_timings.hpp"
//  
// #include "lup_decomposition_test.hpp"
// #include "proto_expression_tests.hpp"
// // 
// #include "tensor_tests.hpp"
// #include "point_example.hpp"
// #include "point_tests.hpp"
// #include "matrix_traits_test.hpp"
// #include "transform_tests.hpp"
// //#include "numeric_cast_traits_test.hpp"
// 
// #include "utility_tests.hpp"
// #include "point_example_multi_type.hpp"
// 
// #include "tolerance_comparison_tests.hpp"
// //#include "constant_tests.hpp"
// #include "distance_tests.hpp"
// #include "bsp_test.hpp"
// #include "member_fusion_adaptor.hpp"
// #include "intersection_tests.hpp"
// 
// #include "reference_frame_tests.hpp"
// 
// //#include "kd_tree_test.hpp"
// // #include "convex_hull_test.hpp"
// #include "segment_interval_test.hpp"
// // #include "trapezoidal_decomposition_test.hpp"
// #include "eberly_triangle_aabb_intersection_tests.hpp"
// //#include "boolean_operation_bsp_test.hpp"
// 
// //#include "compose_matrix_test.hpp"
// 
// #include "vector_point_arithmetic_tests.hpp"
// 
// #include "grid_tests.hpp"
#include "mesh_2d_tests.hpp"
// #include "as_tests.hpp"
// #include "sorting_tests.hpp"
// #include "units_tests.hpp"
// #include "tagged_quantity_tests.hpp"

#include "point_sequence_tests.hpp"
#include "segment_intersection_tests.hpp"

using namespace geometrix;

typedef point_double_2d point2;
typedef vector_double_2d vector2;
typedef segment_double_2d segment2;
typedef polyline<point2> polyline2;
typedef polygon<point2> polygon2;

double vec_length(vector2 const& v)
{
	using namespace geometrix;
	return sqrt(get<0>(v)*get<0>(v) + get<1>(v)*get<1>(v));
}

double p2p_distance(point2 const& p1, point2 const& p2)
{
	using namespace geometrix;
	return point_point_distance(p1, p2);
}

double p2p_angle(point2 const& p1, point2 const& p2)
{
	using namespace geometrix;
	return angle_from_a_to_b(p1, p2);
}

double vec_angle(vector2 const& v)
{
	return geometrix::vector_angle(v);
}

double seg_length(const segment2& seg)
{
	using namespace geometrix;
	return point_point_distance(seg.get_start(), seg.get_end());
}

double seg2p_distance(const segment2& seg, const point2& p)
{
	using namespace geometrix;
	return point_segment_distance(p, seg);
}

void write_point(const point2& p)
{
	logger() << p << std::endl;
}

void write_vector(const vector2& p)
{
	logger() << p << std::endl;
}

void write_segment(const segment2& s)
{
	logger() << s << std::endl;
}

void write_polygon(const polygon2& p)
{
	logger() << p << std::endl;
}

void write_polyline(const polyline2& p)
{
	logger() << p << std::endl;
}

void write_triangle(const point2* p)
{
	logger() << polygon2(p, p + 3) << std::endl;
}

void write_mesh(const geometrix::mesh_2d& mesh)
{
	logger() << "-------start mesh \n";
	for (int i = 0; i < mesh.get_number_triangles(); ++i) {
		write_triangle(mesh.get_triangle_vertices(i));
	}
	logger() << "-------end mesh" << std::endl;
}


void StandardExceptionTranslator(const std::exception& e)
{
	BOOST_TEST_MESSAGE(e.what());
}

boost::unit_test::test_suite* init_unit_test_suite(int, char*[])
{
	boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_messages);
	boost::unit_test::unit_test_monitor.register_exception_translator<std::exception>(&StandardExceptionTranslator);
	boost::unit_test::framework::master_test_suite().p_name.value = "Geometrix Testing Framework";

	//with explicit registration we could specify a test case timeout
	//boost::unit_test::framework::master_test_suite().add( BOOST_TEST_CASE( &infinite_loop ), 0, /* timeout */ 2 );

	return 0;
}