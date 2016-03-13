#ifndef __VARIANT_DETAIL_GET_DATA_HPP
#define __VARIANT_DETAIL_GET_DATA_HPP

namespace variant{
    namespace detail{
        template<typename... ArgsT>
        const typename Variant<ArgsT...>::data_t* get_data(const Variant<ArgsT...>& var){
            return &var.data_;
        }

        template<typename... ArgsT>
        typename Variant<ArgsT...>::data_t* get_data(Variant<ArgsT...>& var){
            return &var.data_;
        }
    }
}

#endif
