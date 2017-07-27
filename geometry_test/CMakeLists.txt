
#! Copyright © 2017
#! Brandon Kohn
#
#  Distributed under the Boost Software License, Version 1.0. (See
#  accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

find_package(Boost 1.60.0)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    message("Boost found: ${Boost_INCLUDE_DIRS}")
    add_definitions(-DBOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK -DBOOST_CHRONO_HEADER_ONLY -D"BOOST_PARAMETER_MAX_ARITY=20")
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    endif()
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# link targets against gtest, gmock
if(Boost_FOUND)
    set(tests
        as_tests
        distance_tests
        eberly_triangle_aabb_intersection_tests
        grid_tests
        intersection_tests
        matrix_traits_test
        mesh_2d_tests
        point_example
        point_sequence_tests
        point_tests
        private_allocator_tests
        product_tests
        proto_expression_tests
        reference_frame_tests
        segment_intersection_tests
        sorting_tests
        tagged_quantity_tests
        tensor_tests
        tolerance_comparison_tests
        transform_tests
        units_tests
        utility_tests
        vector_point_arithmetic_tests
        aabb_intersection_tests
        homogeneous_coordinates_tests
       )

    foreach(test ${tests})
        # add example executable
        add_executable(${test} ${test}.cpp ${test}.hpp)
        target_link_libraries(${test} geometrix)
        add_test(NAME ${test} COMMAND ${test})
    endforeach()
    
endif()