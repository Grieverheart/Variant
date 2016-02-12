#ifndef __VARIANT_HPP
#define __VARIANT_HPP

#include <type_traits>
#include <utility>
#include <cassert>
//#include <new>

namespace variant{

template<typename... ArgsT>
class Variant;

namespace detail{
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

    template<size_t idx, typename T, typename... ArgsT>
    typename std::enable_if<idx >= sizeof...(ArgsT), typename T::result_type>::type
    apply_visitor_helper(const T& visitor, Variant<ArgsT...>& var){
        return detail::forced_return<typename T::result_type>();
    }

    template<size_t idx, typename T, typename... ArgsT>
    typename std::enable_if<idx < sizeof...(ArgsT), typename T::result_type>::type
    apply_visitor_helper(const T& visitor, Variant<ArgsT...>& var){
        using type = typename detail::type_at_index<idx, ArgsT...>::type;
        return visitor(*reinterpret_cast<const type*>(var.data()));
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
        return visitor(*reinterpret_cast<const type_a*>(var_a.data()), *reinterpret_cast<const type_b*>(var_b.data()));
    }

}

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

struct DestructVisitor: public static_visitor<> {
    template<typename T>
    void operator()(T& var)const{
        var.~T();
    }
};

template<typename... ArgsT>
class Variant{
private:
    static const size_t data_size  = detail::static_max<sizeof(ArgsT)...>::value;
    static const size_t data_align = detail::static_max<alignof(ArgsT)...>::value;
    using data_t = typename std::aligned_storage<data_size, data_align>::type;

    int type_id_;
    data_t data_;

public:
    template<typename T, typename Enable = typename std::enable_if<detail::contains<T, ArgsT...>::value>::type>
    Variant(const T& val)
    {
        static_assert(detail::contains<T, ArgsT...>::value, "Error: Type not accepted.");
        new (&data_) T(val);
        type_id_ = detail::type_index<T, ArgsT...>::value;
    }

    template<typename T, typename Enable = typename std::enable_if<detail::contains<T, ArgsT...>::value>::type>
    Variant(T&& val)
    {
        static_assert(detail::contains<T, ArgsT...>::value, "Error: Type not accepted.");
        new (&data_) T(std::move(val));
        type_id_ = detail::type_index<T, ArgsT...>::value;
    }

    Variant(const Variant& val)
    {
        assert(false);
        //TODO: Proper implementation
    }

    Variant(Variant&& val)
    {
        assert(false);
        //TODO: Proper implementation
    }

    ~Variant(void){
        apply_visitor(DestructVisitor(), *this);
    }

    const data_t* data(void)const{
        return &data_;
    }

    int type_id(void)const{
        return type_id_;
    }
};

#define case_call(idx)\
    case idx:{\
        return detail::apply_visitor_helper<idx, T, ArgsT...>(visitor, var);\
    }\

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
        default: return detail::forced_return<typename T::result_type>();
    }
}

}

#endif
