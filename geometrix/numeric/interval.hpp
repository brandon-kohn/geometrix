//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/numeric/detail/friend_ordered_field_operators.hpp>
#include <geometrix/numeric/constants.hpp>
#include <stk/container/icl/interval.hpp>
#include <boost/numeric/interval.hpp>
#include <boost/type_traits.hpp>
#include <limits>

namespace geometrix {

    template <typename Domain>
    class interval;

    #define GEOMETRIX_IMPLEMENT_ARITHMETIC_OP(Op)\
        template <typename U, typename std::enable_if<!std::is_same<interval<T>, U>::value, int>::type = 0>\
        interval<T>& operator Op(const U& rhs)\
        {\
            self()->internal_rep() Op rhs;\
            return *self();\
        }\
        template <typename U>\
        interval<T>& operator Op(const interval<U>& rhs)\
        {\
            self()->internal_rep() Op rhs.internal_rep();\
            return *self();\
        }\
    /***/

    namespace detail{

        template <typename Domain>
        struct interval_storage
        {
            using domain_type = Domain;

            interval_storage()
                : m_lower{ std::numeric_limits<domain_type>::has_infinity ? std::numeric_limits<domain_type>::infinity() : constants::one<domain_type>() }
                , m_upper{ std::numeric_limits<domain_type>::has_infinity ? -std::numeric_limits<domain_type>::infinity() : constants::zero<domain_type>() }
            {}

            interval_storage(const domain_type& v)
                : m_lower{ v }
                , m_upper{ v }
            {}

            interval_storage(const domain_type& l, const domain_type& u)
                : m_lower{ l }
                , m_upper{ u }
            {}

            bool is_empty() const
            {
                return !(m_lower <= m_upper);
            }

            const domain_type& lower() const
            {
                return m_lower;
            }

            const domain_type& upper() const
            {
                return m_upper;
            }

            void set_lower(const domain_type& v)
            {
                m_lower = v;
            }

            void set_upper(const domain_type& v)
            {
                m_upper = v;
            }

            void set(const domain_type& l, const domain_type& u)
            {
                m_lower = l;
                m_upper = u;
            }

            interval_storage& operator +=(const domain_type& v)
            {
                m_lower += v;
                m_upper += v;
                return *this;
            }

            interval_storage& operator +=(const interval_storage& v)
            {
                m_lower += v.m_lower;
                m_upper += v.m_upper;
                return *this;
            }

            interval_storage& operator -=(const domain_type& v)
            {
                m_lower -= v;
                m_upper -= v;
                return *this;
            }

            interval_storage& operator -=(const interval_storage& v)
            {
                m_lower -= v.m_lower;
                m_upper -= v.m_upper;
                return *this;
            }

            interval_storage& operator *=(const domain_type& v)
            {
                m_lower *= v;
                m_upper *= v;
                return *this;
            }

            interval_storage& operator /=(const domain_type& v)
            {
                m_lower /= v;
                m_upper /= v;
                return *this;
            }

        private:

            domain_type m_lower;
            domain_type m_upper;
        };

        template <typename Derived>
        struct crtp_base
        {
            Derived* self() { return static_cast<Derived*>(this); }
            const Derived* self() const { return static_cast<const Derived*>(this); }
        };

        template<typename Derived, typename Base = crtp_base<Derived>, typename EnableIf=void>
        struct interval_plus : Base{};

        template<typename T, typename Base>
        struct interval_plus<interval<T>, Base, typename std::enable_if<boost::has_plus<typename std::decay<T>::type>::value>::type>
        : Base
        {
            using crtp_base<interval<T>>::self;
            GEOMETRIX_IMPLEMENT_ARITHMETIC_OP(+=);
            GEOMETRIX_FRIEND_BINARY_OPERATOR_COMMUTATIVE(interval<T>, T, +);
            GEOMETRIX_FRIEND_BINARY_OPERATOR_COMMUTATIVE_SELF(interval<T>, +);
        };

        template<typename Derived, typename Base = crtp_base<Derived>, typename EnableIf=void>
        struct interval_minus : Base{};

        template<typename T, typename Base>
        struct interval_minus<interval<T>, Base, typename std::enable_if<boost::has_minus<typename std::decay<T>::type>::value>::type>
        : Base
        {
            using crtp_base<interval<T>>::self;
            GEOMETRIX_IMPLEMENT_ARITHMETIC_OP(-=);
            GEOMETRIX_FRIEND_BINARY_OPERATOR_NON_COMMUTATIVE(interval<T>, T, -);
            GEOMETRIX_FRIEND_BINARY_OPERATOR_NON_COMMUTATIVE_SELF(interval<T>, -);
        };

        template<typename Derived, typename Base = crtp_base<Derived>, typename EnableIf=void>
        struct interval_multiplies : Base{};

        template<typename T, typename Base>
        struct interval_multiplies<interval<T>, Base, typename std::enable_if<boost::has_multiplies<typename std::decay<T>::type>::value>::type>
        : Base
        {
            using crtp_base<interval<T>>::self;
            GEOMETRIX_IMPLEMENT_ARITHMETIC_OP(*=);
            GEOMETRIX_FRIEND_BINARY_OPERATOR_COMMUTATIVE(interval<T>, T, *);
            //GEOMETRIX_FRIEND_BINARY_OPERATOR_COMMUTATIVE_SELF(interval<T>, *);
        };

        template<typename Derived, typename Base = crtp_base<Derived>, typename EnableIf=void>
        struct interval_divides : Base{};

        template<typename T, typename Base>
        struct interval_divides<interval<T>, Base, typename std::enable_if<boost::has_divides<typename std::decay<T>::type>::value>::type>
        : Base
        {
            using crtp_base<interval<T>>::self;
            GEOMETRIX_FRIEND_BINARY_OPERATOR_NON_COMMUTATIVE(interval<T>, T, /);
            //GEOMETRIX_FRIEND_BINARY_OPERATOR_NON_COMMUTATIVE_SELF(interval<T>, /);
            GEOMETRIX_IMPLEMENT_ARITHMETIC_OP(/=);
        };
    }//! namespace detail;

    //! By default the storage type uses boost::numeric_interval. This works fine for arithmetic types and boost units.
    //! This should be specialized for things like vectors used to represent angle ranges.
    template <typename T, typename EnableIf=void>
    struct interval_storage_type
    {
        using type = detail::interval_storage<T>;
    };

    template <typename Domain>
    class interval
    : public detail::interval_plus<interval<Domain>
    , detail::interval_minus<interval<Domain>
    , detail::interval_divides<interval<Domain>
    , detail::interval_multiplies<interval<Domain>
      >>>>
    {
    public:

        using domain_type = Domain;
        using interval_bounds = stk::icl::interval_bounds;
        using internal_rep_type = typename interval_storage_type<domain_type>::type;
        using bounded_domain_type = typename stk::icl::bounded_value<Domain>::type;
        BOOST_CONCEPT_ASSERT((boost::DefaultConstructibleConcept<domain_type>));

        static interval open(const domain_type& lo, const domain_type& up){ return interval(lo, up, interval_bounds::open()); }
        static interval closed(const domain_type& lo, const domain_type& up){ return interval(lo, up, interval_bounds::closed()); }
        static interval closed(const domain_type& v){ return interval(v, v, interval_bounds::closed()); }
        static interval right_open(const domain_type& lo, const domain_type& up){ return interval(lo, up, interval_bounds::right_open()); }
        static interval left_open(const domain_type& lo, const domain_type& up){ return interval(lo, up, interval_bounds::left_open()); }

        interval()
            : m_interval{}
            , m_bounds(interval_bounds::right_open())
        {}

        explicit interval(const domain_type& v)
            : m_interval(v)
            , m_bounds(interval_bounds::closed())
        {}

        template <typename T>
        interval(const boost::numeric::interval<T>& v)
            : m_interval(v)
            , m_bounds(interval_bounds::closed())
        {}

        interval(const domain_type& low, const domain_type& up, stk::icl::interval_bounds bounds = stk::icl::interval_bounds::right_open())
            : m_interval(low, up)
            , m_bounds(bounds)
        {}

        domain_type     lower() const { return m_interval.lower(); }
        domain_type     upper() const { return m_interval.upper(); }
        interval_bounds bounds() const { return m_bounds; }

        bool            is_empty() const
        {
            return m_interval.is_empty();
        }

        interval&       expand(const domain_type& v)
        {
            auto was_empty = is_empty();
            if (!was_empty && v < lower())
                m_interval.set_lower(v);
            else if(!was_empty && v > upper())
                m_interval.set_upper(v);
            else if (was_empty)
                m_interval.set(v, v);
            return *this;
        }

        template <typename NumberComparisonPolicy>
        interval&       expand(const domain_type& v, const NumberComparisonPolicy& cmp)
        {
            if (cmp.less_than(v, lower()))
                m_interval.set(v, upper());
            else if(cmp.greater_than(v, upper()))
                m_interval.set(lower(), v);
            return *this;
        }

        template <typename U, typename NumberComparisonPolicy>
        bool intersects(const interval<U>& rhs, const NumberComparisonPolicy& cmp) const
        {
            return cmp.less_than(rhs.lower(), upper()) && cmp.less_than(lower(), rhs.upper());
        }

        template <typename U>
        bool intersects(const interval<U>& rhs) const
        {
            return (rhs.lower() < upper()) && (lower() < rhs.upper());
        }

        internal_rep_type& internal_rep()
        {
            return m_interval;
        }

    private:
        internal_rep_type         m_interval;
        stk::icl::interval_bounds m_bounds;
    };
}//! namespace geometrix;

namespace stk { namespace icl {

    template<typename DomainT>
    struct interval_traits<geometrix::interval<DomainT>>
    {
        using type = interval_traits;
        using domain_type = DomainT;
        using domain_compare = std::less<domain_type>;
        using interval_type = geometrix::interval<DomainT>;

        static interval_type construct(const domain_type& lo, const domain_type& up)
        {
            return interval_type(lo, up);
        }

        static domain_type lower(const interval_type& inter_val){ return inter_val.lower(); };
        static domain_type upper(const interval_type& inter_val){ return inter_val.upper(); };
    };

    template<typename DomainT>
    struct dynamic_interval_traits<geometrix::interval<DomainT> >
    {
        using type = dynamic_interval_traits;
        using interval_type = geometrix::interval<DomainT>;
        using domain_type = DomainT;
        using domain_compare = std::less<domain_type>;

        static interval_type construct(const domain_type& lo, const domain_type& up, interval_bounds bounds)
        {
            return interval_type(lo, up, bounds);
        }

        static interval_type construct_bounded(const bounded_value<DomainT>& lo, const bounded_value<DomainT>& up)
        {
            return interval_type(lo.value(), up.value(), lo.bound().left() | up.bound().right());
        }
    };

    template <typename DomainT>
    struct interval_bound_type<geometrix::interval<DomainT>>
    {
        using type = interval_bound_type<geometrix::interval<DomainT>>;
        BOOST_STATIC_CONSTANT(bound_type, value = interval_bounds::dynamic);
    };

    template <typename DomainT>
    struct is_interval<geometrix::interval<DomainT>>
    {
        using type = is_interval<geometrix::interval<DomainT>>;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

    template <typename Domain>
    struct is_continuous_interval<geometrix::interval<Domain>>
    {
        using type = is_continuous_interval<geometrix::interval<Domain>>;
        BOOST_STATIC_CONSTANT(bool, value = is_continuous<Domain>::value);
    };

    template <typename Domain>
    struct is_discrete_interval<geometrix::interval<Domain>>
    {
        using type = is_discrete_interval<geometrix::interval<Domain>>;
        BOOST_STATIC_CONSTANT(bool, value = is_discrete<Domain>::value);
    };

    template <typename DomainT>
    struct type_to_string<geometrix::interval<DomainT>>
    {
        static std::string apply() { return "cI<"+ type_to_string<DomainT>::apply() +">"; }
    };

    template<typename DomainT>
    struct value_size<geometrix::interval<DomainT> >
    {
        static std::size_t apply(const geometrix::interval<DomainT>&) { return 2; }
    };

}}//! namespace stk::icl;

#undef GEOMETRIX_IMPLEMENT_ARITHMETIC_OP
