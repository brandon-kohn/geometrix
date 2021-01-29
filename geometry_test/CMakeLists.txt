
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
    set(boost_tests
        kd_tree_test
        as_tests
        distance_tests
        eberly_triangle_aabb_intersection_tests
        grid_tests
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
        intersection_tests
    )

    foreach(test ${boost_tests})
        add_executable(${test} ${test}.cpp ${test}.hpp)
        if(MSVC)
            target_compile_options(${test} PRIVATE /W4 -wd4127)
        else()
            target_compile_options(${test} PRIVATE -Wall -Wextra -Wno-unused-local-typedefs -Wno-missing-braces)
        endif()
        target_link_libraries(${test} gtest_static geometrix stk)
        add_test(NAME ${test} COMMAND ${test})
    endforeach()
    
    # Use google tests.
    set(gtests
        bsp_test
        capsule_tests
        gtest_intersection_tests
    )
    
    foreach(test ${gtests})
        add_executable(${test} ${test}.cpp)
        if(MSVC)
            target_compile_options(${test} PRIVATE /W4 -wd4127)
        else()
            target_compile_options(${test} PRIVATE -Wall -Wextra -Wno-unused-local-typedefs -Wno-missing-braces)
        endif()
        target_link_libraries(${test} gtest_static gmock_main_static geometrix stk)
        add_test(NAME ${test} COMMAND ${test})
    endforeach()
        
    set(compile_fail_tests
        numeric_sequence_fail_construction
    )
    
    foreach(test ${compile_fail_tests})
        add_executable(${test} ${test}.cpp)
        target_link_libraries(${test} gtest_static gmock_main_static geometrix stk)
        set_target_properties(${test} PROPERTIES
                                      EXCLUDE_FROM_ALL TRUE
                                      EXCLUDE_FROM_DEFAULT_BUILD TRUE)
        add_test(NAME ${test} COMMAND ${CMAKE_COMMAND} --build . --target ${test} --config $<CONFIGURATION>
                              WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        set_tests_properties(${test} PROPERTIES WILL_FAIL TRUE)
    endforeach()
endif()
