#ifndef __VARIANT_APPLY_VISITOR_HPP
#define __VARIANT_APPLY_VISITOR_HPP

#include "detail/get_data.hpp"
#include "detail/type_at_index.hpp"
#include "detail/forced_return.hpp"

namespace variant{ namespace detail{

    template<size_t idx, typename T, typename... ArgsT>
    typename std::enable_if<idx >= sizeof...(ArgsT), typename T::result_type>::type
    apply_visitor_helper(const T& visitor, const Variant<ArgsT...>& var){
        return detail::forced_return<typename T::result_type>();
    }

    template<size_t idx, typename T, typename... ArgsT>
    typename std::enable_if<idx < sizeof...(ArgsT), typename T::result_type>::type
    apply_visitor_helper(const T& visitor, const Variant<ArgsT...>& var){
        using type = typename detail::type_at_index<idx, ArgsT...>::type;
        return visitor(*reinterpret_cast<const type*>(get_data(var)));
    }

    template<size_t idx_a, size_t idx_b, typename T, typename... ArgsT>
    typename std::enable_if<(idx_a >= sizeof...(ArgsT)) || (idx_b >= sizeof...(ArgsT)), typename T::result_type>::type
    apply_visitor_helper(const T& visitor, const Variant<ArgsT...>& var_a, const Variant<ArgsT...>& var_b){
        return detail::forced_return<typename T::result_type>();
    }

    template<size_t idx_a, size_t idx_b, typename T, typename... ArgsT>
    typename std::enable_if<(idx_a < sizeof...(ArgsT)) && (idx_b < sizeof...(ArgsT)), typename T::result_type>::type
    apply_visitor_helper(const T& visitor, const Variant<ArgsT...>& var_a, const Variant<ArgsT...>& var_b){
        using type_a = typename detail::type_at_index<idx_a, ArgsT...>::type;
        using type_b = typename detail::type_at_index<idx_b, ArgsT...>::type;
        return visitor(*reinterpret_cast<const type_a*>(get_data(var_a)), *reinterpret_cast<const type_b*>(get_data(var_b)));
    }

    template<size_t idx, typename T, typename... ArgsT>
    typename std::enable_if<idx >= sizeof...(ArgsT), typename T::result_type>::type
    apply_visitor_helper(const T& visitor, Variant<ArgsT...>& var){
        return detail::forced_return<typename T::result_type>();
    }

    template<size_t idx, typename T, typename... ArgsT>
    typename std::enable_if<idx < sizeof...(ArgsT), typename T::result_type>::type
    apply_visitor_helper(const T& visitor, Variant<ArgsT...>& var){
        using type = typename detail::type_at_index<idx, ArgsT...>::type;
        return visitor(*reinterpret_cast<type*>(get_data(var)));
    }

    template<size_t idx_a, size_t idx_b, typename T, typename... ArgsT>
    typename std::enable_if<(idx_a >= sizeof...(ArgsT)) || (idx_b >= sizeof...(ArgsT)), typename T::result_type>::type
    apply_visitor_helper(const T& visitor, Variant<ArgsT...>& var_a, Variant<ArgsT...>& var_b){
        return detail::forced_return<typename T::result_type>();
    }

    template<size_t idx_a, size_t idx_b, typename T, typename... ArgsT>
    typename std::enable_if<(idx_a < sizeof...(ArgsT)) && (idx_b < sizeof...(ArgsT)), typename T::result_type>::type
    apply_visitor_helper(const T& visitor, Variant<ArgsT...>& var_a, Variant<ArgsT...>& var_b){
        using type_a = typename detail::type_at_index<idx_a, ArgsT...>::type;
        using type_b = typename detail::type_at_index<idx_b, ArgsT...>::type;
        return visitor(*reinterpret_cast<type_a*>(get_data(var_a)), *reinterpret_cast<type_b*>(get_data(var_b)));
    }
} //namespace detail

    template<typename... T>
    struct static_visitor;

    template<>
    struct static_visitor<>{
        using result_type = void;
    };

    template<typename T>
    struct static_visitor<T>{
        using result_type = T;
    };

#define case_call(idx)\
        case idx:{\
            return detail::apply_visitor_helper<idx, T, ArgsT...>(visitor, var);\
        }\

    template<typename T, typename... ArgsT>
    typename T::result_type apply_visitor(const T& visitor, const Variant<ArgsT...>& var){
        switch(var.type_id()){
            case_call(0);
            case_call(1);
            case_call(2);
            case_call(3);
            case_call(4);
            case_call(5);
            case_call(6);
            case_call(7);
            case_call(8);
            case_call(9);
            case_call(10);
            case_call(11);
            case_call(12);
            case_call(13);
            case_call(14);
            case_call(15);
            default: return detail::forced_return<typename T::result_type>();
        }
    }

    template<typename T, typename... ArgsT>
    typename T::result_type apply_visitor(const T& visitor, Variant<ArgsT...>& var){
        switch(var.type_id()){
            case_call(0);
            case_call(1);
            case_call(2);
            case_call(3);
            case_call(4);
            case_call(5);
            case_call(6);
            case_call(7);
            case_call(8);
            case_call(9);
            case_call(10);
            case_call(11);
            case_call(12);
            case_call(13);
            case_call(14);
            case_call(15);
            default: return detail::forced_return<typename T::result_type>();
        }
    }

#define case_call_b(idx_a, idx_b)\
        case idx_b:{\
            return detail::apply_visitor_helper<idx_a, idx_b, T, ArgsT...>(visitor, var_a, var_b);\
        }\

#define case_call_a(idx_a)\
        case idx_a:\
            switch(var_b.type_id()){\
                case_call_b(idx_a, 0);\
                case_call_b(idx_a, 1);\
                case_call_b(idx_a, 2);\
                case_call_b(idx_a, 3);\
                case_call_b(idx_a, 4);\
                case_call_b(idx_a, 5);\
                case_call_b(idx_a, 6);\
                case_call_b(idx_a, 7);\
                case_call_b(idx_a, 8);\
                case_call_b(idx_a, 9);\
                case_call_b(idx_a, 10);\
                case_call_b(idx_a, 11);\
                case_call_b(idx_a, 12);\
                case_call_b(idx_a, 13);\
                case_call_b(idx_a, 14);\
                case_call_b(idx_a, 15);\
                default: return detail::forced_return<typename T::result_type>();\
            }

    template<typename T, typename... ArgsT>
    typename T::result_type apply_visitor(const T& visitor, Variant<ArgsT...>& var_a, Variant<ArgsT...>& var_b){
        switch(var_a.type_id()){
            case_call_a(0);
            case_call_a(1);
            case_call_a(2);
            case_call_a(3);
            case_call_a(4);
            case_call_a(5);
            case_call_a(6);
            case_call_a(7);
            case_call_a(8);
            case_call_a(9);
            case_call_a(10);
            case_call_a(11);
            case_call_a(12);
            case_call_a(13);
            case_call_a(14);
            case_call_a(15);
            default: return detail::forced_return<typename T::result_type>();
        }
    }

    template<typename T, typename... ArgsT>
    typename T::result_type apply_visitor(const T& visitor, const Variant<ArgsT...>& var_a, const Variant<ArgsT...>& var_b){
        switch(var_a.type_id()){
            case_call_a(0);
            case_call_a(1);
            case_call_a(2);
            case_call_a(3);
            case_call_a(4);
            case_call_a(5);
            case_call_a(6);
            case_call_a(7);
            case_call_a(8);
            case_call_a(9);
            case_call_a(10);
            case_call_a(11);
            case_call_a(12);
            case_call_a(13);
            case_call_a(14);
            case_call_a(15);
            default: return detail::forced_return<typename T::result_type>();
        }
    }
} //namespace variant

#endif
