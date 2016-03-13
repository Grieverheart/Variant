#ifndef __VARIANT_DETAIL_FORCED_RETURN_HPP
#define __VARIANT_DETAIL_FORCED_RETURN_HPP

#include <cassert>

namespace variant{ namespace detail{

    template <typename T>
    inline T forced_return()
    {
        // logical error: should never be here! (see above)
        assert(false);

        T (*dummy_function_ptr)() = 0;
        return dummy_function_ptr();
    }

    template <>
    inline void forced_return<void>()
    {
        // logical error: should never be here! (see above)
        assert(false);
    }
}}

#endif
