//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_UTILITY_PRIVATE_ALLOCATOR_HPP
#define GEOMETRIX_UTILITY_PRIVATE_ALLOCATOR_HPP

#include <cstddef>
#include <climits>

namespace geometrix {
    namespace detail {
        template <typename T, template <typename> class Derived>
        struct private_allocator
        {
        public:
            typedef T              value_type;
            typedef T*             pointer;
            typedef const T*       const_pointer;
            typedef T&             reference;
            typedef const T&       const_reference;
            typedef std::size_t    size_type;
            typedef std::ptrdiff_t difference_type;

            private_allocator() throw() {}
            private_allocator(const private_allocator&) throw() {}
            template <class U, template <typename> class D2>
            private_allocator(const private_allocator<U, D2>&) throw() {}
            ~private_allocator() throw() {}

            template <typename U>
            struct rebind
            {
                typedef Derived<U> other;
            };

            pointer address(reference value) const
            {
                return &value;
            }

            const_pointer address(const_reference value) const
            {
                return &value;
            }

            size_type max_size() const throw()
            {
                return (std::numeric_limits<std::size_t>::max)() / sizeof(T);
            }

            pointer allocate(size_type num, const void* = 0)
            {
                return (pointer)(::operator new(num * sizeof(T)));
            }

            void deallocate(pointer p, size_type num)
            {
                ::operator delete((void*)p);
            }
        };

        template <typename T, template <typename> class Derived1, typename U, template <typename> class Derived2>
        inline bool operator==(const private_allocator<T, Derived1>&, const private_allocator<U, Derived2>&)
        {
            return true;
        }

        template <typename T, template <typename> class Derived1, typename U, template <typename> class Derived2>
        inline bool operator!=(const private_allocator<T, Derived1>&, const private_allocator<U, Derived2>&)
        {
            return false;
        }

    }//! namespace detail;
}//! namespace geometrix;

#define GEOMETRIX_PRIVATE_ALLOCATOR(Name)                \
    template <typename T>                                \
    struct Name                                          \
    : geometrix::detail::private_allocator<T, Name>      \
    {                                                    \
        Name() throw() {}                                \
        Name(const Name&) throw() {}                     \
        template <class U>                               \
        Name(const Name<U>&) throw() {}                  \
        void construct(pointer p, const T& value)        \
        {                                                \
            new((void*)p)T(value);                       \
        }                                                \
        void destroy(pointer p)                          \
        {                                                \
            p->~T();                                     \
        }                                                \
    };                                                   \
/***/

#endif//! GEOMETRIX_UTILITY_PRIVATE_ALLOCATOR_HPP