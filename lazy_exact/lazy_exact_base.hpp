//  Boost lazy_exact_filter.hpp header file  ------------------------------------------//

//  (C) Copyright Brandon Kohn 2005.

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or
//   copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef _LAZY_EXACT_BASE_HPP
#define _LAZY_EXACT_BASE_HPP

#include <boost/numeric/interval.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/call_traits.hpp>

namespace boost
{
namespace numeric
{
    template <typename T, typename F>
    class lazy_exact_number;

    namespace detail
    {
        template<typename Filter, typename Exact>
        class exact_provider 
        {
        public:

            exact_provider() 
                :m_counter(0)
            {}

            virtual ~exact_provider(){}

            typedef Filter                                                            filter_type;
            typedef Exact                                                             exact_type;
            typedef boost::intrusive_ptr< exact_provider< filter_type, exact_type > > pointer;

            virtual bool                                           is_exact() const = 0;
            virtual exact_type                                     get_exact() const = 0;
            virtual const boost::numeric::interval< filter_type >& approximate_value() const = 0;
            virtual inline  pointer                                clone() const = 0;

            inline size_t refcnt(void) const { return m_counter; }
            inline void ref(void) const { ++m_counter; }
            inline void unref(void) const { --m_counter; }

        private:

            mutable size_t                                  m_counter;

        };

        template <typename FilterType, typename ExactType>
        void intrusive_ptr_add_ref( exact_provider<FilterType, ExactType>* r )
        {
            r->ref();
        }

        template <typename FilterType, typename ExactType>
        void intrusive_ptr_release( exact_provider<FilterType, ExactType>* r )
        {
            r->unref();
            if (r->refcnt() == 0)
                delete r;
        }

        template<typename LazyType>
        class lazy_exact_representation: public exact_provider< typename LazyType::filter_type, typename LazyType::exact_type > 
        {
        public:

            typedef LazyType                       lazy_type;
            typedef typename LazyType::filter_type filter_type;
            typedef typename LazyType::exact_type  exact_type;
            typedef typename boost::is_same< lazy_type, lazy_exact_number< filter_type, exact_type > >::type is_exact_t;
            typedef typename exact_provider< filter_type, exact_type >::pointer pointer;

            lazy_exact_representation( const LazyType& value )
                : m_lazyValue( value )
            {}

            inline bool                                           is_exact() const { return is_exact_t::value;  }
            inline exact_type                                     get_exact() const { return m_lazyValue.get_exact(); }
            inline const boost::numeric::interval< filter_type >& approximate_value() const { return m_lazyValue.approximate_value(); }
            inline pointer                                        clone() const { return pointer( new lazy_exact_representation< LazyType >( m_lazyValue ) );  }

        private:

            LazyType m_lazyValue;

        };

        template <typename LazyType>
        typename exact_provider< typename LazyType::filter_type, typename LazyType::exact_type >::pointer make_lazy_exact_representation( const LazyType& value )
        {
            return typename exact_provider< typename LazyType::filter_type, typename LazyType::exact_type >::pointer( new lazy_exact_representation< LazyType >( value ) );
        }
    }

//////////////////////////////////////////////
//
// CLASS lazy_exact_filter
//
// Base class and interface to the lazy_exact number type.
//
template <typename FilterType>
class lazy_exact_filter
{
public:
		
    typedef FilterType                                             filter_type;    
    typedef typename boost::call_traits< filter_type >::param_type filter_param;

	lazy_exact_filter( filter_param value, filter_param filterPrecision )
        : m_interval( value-(filterPrecision*value),value+(filterPrecision*value) )        
    {}

	lazy_exact_filter( const boost::numeric::interval< filter_type >& interval )
        : m_interval(interval)        
    {}
	
	///TODO: Need conversion from exact to filter type to set interval.
	lazy_exact_filter( filter_param filter )
        : m_interval(filter,filter)        
    {}

	virtual ~lazy_exact_filter(){}

	///Access the interval approximation
	inline const boost::numeric::interval< filter_type >& approximate_value() const { return m_interval; }
	
private:
    
	mutable boost::numeric::interval< filter_type > m_interval;

};

}}///namespace boost::numeric

#endif //_LAZY_EXACT_BASE_TYPE_HPP
