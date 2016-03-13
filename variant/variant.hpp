#ifndef __VARIANT_HPP
#define __VARIANT_HPP

#include <type_traits>
#include <utility>

#include "variant_fwd.hpp"
#include "apply_visitor.hpp"
#include "detail/static_max.hpp"

namespace variant{

struct DestructVisitor: public static_visitor<> {
    template<typename T>
    void operator()(T& var)const{
        var.~T();
    }
};

template<typename T>
struct CopyVisitor: public static_visitor<T> {
    template<typename U>
    T operator()(const U& var)const{
        T data;
        new (&data) U(var);
        return data;
    }
};

template<typename... ArgsT>
class Variant{
private:
    static const size_t data_size  = detail::static_max<sizeof(ArgsT)...>::value;
    static const size_t data_align = detail::static_max<alignof(ArgsT)...>::value;
public:
    using data_t = typename std::aligned_storage<data_size, data_align>::type;

private:
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

    Variant(const Variant& val):
        type_id_(val.type_id_),
        data_(apply_visitor(CopyVisitor<data_t>(), val))
    {}

    Variant(Variant&& val):
        type_id_(val.type_id_),
        data_(val.data_)
    {
        val.type_id_ = -1;
    }

    ~Variant(void){
        if(type_id_ >= 0) apply_visitor(DestructVisitor(), *this);
    }

    int type_id(void)const{
        return type_id_;
    }

    template<typename... ArgsV>
    friend
    const typename Variant<ArgsV...>::data_t* detail::get_data(const Variant<ArgsV...>& var);
    template<typename... ArgsV>
    friend
    typename Variant<ArgsV...>::data_t* detail::get_data(Variant<ArgsV...>& var);
};

} //namespace variant

#endif
