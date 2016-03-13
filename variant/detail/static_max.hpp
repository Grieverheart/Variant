#ifndef __VARIANT_DETAIL_STATIC_MAX_HPP
#define __VARIANT_DETAIL_STATIC_MAX_HPP

namespace variant{namespace detail{
    template <size_t arg1, size_t ... others>
    struct static_max;

    template <size_t arg>
    struct static_max<arg>{
        static const size_t value = arg;
    };

    template <size_t arg1, size_t arg2, size_t ... others>
    struct static_max<arg1, arg2, others...>{
        static const size_t value = arg1 >= arg2 ? static_max<arg1, others...>::value :
        static_max<arg2, others...>::value;
    };
}}

#endif
