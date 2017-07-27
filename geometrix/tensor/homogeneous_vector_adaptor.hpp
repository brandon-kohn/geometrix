//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_HOMOGENEOUS_VECTOR_ADAPTOR_HPP
#define GEOMETRIX_HOMOGENEOUS_VECTOR_ADAPTOR_HPP

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/numeric/constants.hpp>

namespace geometrix {

//! Treat a vector as a point.
template <typename Vector, typename HomogeneousCoordPolicy>
struct homogeneous_vector_adaptor
{
    GEOMETRIX_STATIC_ASSERT(( is_vector<Vector>::value ));
    BOOST_STATIC_CONSTANT(int, homogeneous_index = dimension_of<Vector>::value);
    using vector_type = Vector;
    using homogeneous_policy_type = HomogeneousCoordPolicy;
    
    homogeneous_vector_adaptor( const Vector& p) 
        : m_vec(p)
    {}
    
    template <int Index>
    typename type_at<Vector, Index>::type get() const
    {
        return geometrix::get<Index>(m_vec);
    }
    
    template <>
    typename HomogeneousCoordPolicy::type get<homogeneous_index>() const
    {
        return HomogeneousCoordPolicy().get();
    }
    
private:
    
    const Vector& m_vec;

};

template <typename Coord>
struct positional_homogeneous_coord_policy
{
    using type = Coord;
    
    type get() const
    {
        return constants::one<type>();
    }
};

template <typename Coord>
struct normalized_homogeneous_coord_policy
{
    using type = Coord;
    
    type get() const
    {
        return constants::zero<type>();
    }
};

template <typename Vector, typename HomogeneousCoordPolicy>
struct is_vector< homogeneous_vector_adaptor<Vector, HomogeneousCoordPolicy> > : boost::true_type{};

template <typename Sequence, typename Policy>
struct geometric_traits< homogeneous_vector_adaptor< Sequence, Policy > > 
    : diversity_base< Sequence >
{
    typedef homogeneous_vector_adaptor<Sequence, Policy>         vector_type;
    typedef void                                                 is_vector;
    typedef typename geometric_traits<Sequence>::reference_frame reference_frame;
    typedef void                                                 is_coordinate_sequence;
    typedef typename geometric_traits<Sequence>::arithmetic_type arithmetic_type;
    using dimensionless_type = typename geometric_traits<Sequence>::dimensionless_type;
    typedef void                                                 is_numeric_sequence;
    typedef Sequence                                             sequence_type;
    typedef dimension<dimension_of<Sequence>::value + 1>         dimension_type;
    typedef void                                                 is_sequence;
};

template <typename T>
struct homogeneous_vector_adaptor_access_policy
{
    typedef void compile_time_access;
    BOOST_STATIC_CONSTANT(int, homogeneous_index = T::homogeneous_index);

    template <std::size_t Index>
    struct type_at 
    : ::geometrix::type_at<typename T::vector_type, Index>
    {};
    
    template <>
    struct type_at<homogeneous_index>
    : T::homogeneous_policy_type
    {};

    //! \brief compile time access if available for the collection.
    template <std::size_t Index>
    static typename type_at<Index>::type get( const T& collection ) 
    {
        return collection.template get<Index>();
    }

    //! \brief compile time access if available for the collection.
    //template <std::size_t Index>
    //static void set( T& collection, typename boost::call_traits<typename type_at<Index>::type>::param_type v )
    //{
    //    collection.template set<Index>(v);
    //}
};

template <typename Coord, typename Vector>
inline homogeneous_vector_adaptor<Vector, positional_homogeneous_coord_policy<Coord>> as_positional_homogeneous( const Vector& p )
{
    return homogeneous_vector_adaptor<Vector, positional_homogeneous_coord_policy<Coord>>(p);
}

template <typename Coord, typename Vector>
inline homogeneous_vector_adaptor<Vector, normalized_homogeneous_coord_policy<Coord>> as_normalized_homogeneous( const Vector& p )
{
    return homogeneous_vector_adaptor<Vector, normalized_homogeneous_coord_policy<Coord>>(p);
}

template <typename Vector, typename Policy>
struct tensor_traits< homogeneous_vector_adaptor<Vector, Policy> > 
    : tensor_traits< typename remove_const_ref<Vector>::type >
{
    typedef AccessPolicy        access_policy;
    typedef boost::mpl::int_<1> tensor_order;
    typedef void                GEOMETRIX_TENSOR_ORDER(1);
    typedef void                is_tensor;
};

}//namespace geometrix;

#endif //GEOMETRIX_HOMOGENEOUS_VECTOR_ADAPTOR_HPP
