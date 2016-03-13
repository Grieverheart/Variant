#ifndef __VARIANT_GET_HPP
#define __VARIANT_GET_HPP

#include "detail/get_data.hpp"

namespace variant{

template<typename T, typename... ArgsT>
const T& unsafe_get(const Variant<ArgsT...>& var){
    return *reinterpret_cast<const T*>(detail::get_data(var));
}

template<typename T, typename... ArgsT>
T& unsafe_get(Variant<ArgsT...>& var){
    return *reinterpret_cast<T*>(detail::get_data(var));
}

template<typename T, typename... ArgsT>
const T* unsafe_get(const Variant<ArgsT...>* var){
    return reinterpret_cast<const T*>(detail::get_data(*var));
}

template<typename T, typename... ArgsT>
T* unsafe_get(Variant<ArgsT...>* var){
    return reinterpret_cast<T*>(detail::get_data(*var));
}

} //namespace variant

#endif
