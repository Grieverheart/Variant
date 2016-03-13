#ifndef __VARIANT_DETAIL_TYPE_AT_INDEX_HPP
#define __VARIANT_DETAIL_TYPE_AT_INDEX_HPP

namespace variant{ namespace detail{
    template<int idx, typename T, typename... ArgsT>
    struct type_index_helper;

    template<int idx, typename T>
    struct type_index_helper<idx, T>{
        static const int value = -1;
    };

    template<int idx, typename T, typename A, typename... ArgsT>
    struct type_index_helper<idx, T, A, ArgsT...>{
        static const int value = (std::is_same<T, A>::value)? idx: type_index_helper<idx + 1, T, ArgsT...>::value;
    };

    template<typename T, typename... ArgsT>
    struct type_index{
        static const int value = type_index_helper<0, T, ArgsT...>::value;
    };

    template<typename T, typename... ArgsT>
    struct contains{
        static const bool value = (type_index<T, ArgsT...>::value >= 0)? true: false;
    };

    template<size_t idx, typename ...ArgsT>
    struct type_at_index;

    template<size_t idx>
    struct type_at_index<idx>{
        using type = int;
    };

    template<typename T, typename ...ArgsT>
    struct type_at_index<0, T, ArgsT...>{
        using type = T;
    };

    template<size_t idx, typename T, typename ...ArgsT>
    struct type_at_index<idx, T, ArgsT...>{
        using type = typename type_at_index<idx - 1, ArgsT...>::type;
    };
}}

#endif
