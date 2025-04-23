#pragma once
#include<cstdint>


namespace UVENGINE {

    namespace METAPROG {

    // Template type metafunction.
    template<typename TYPE>
    struct type_id { using type = TYPE; };

    // Template value metafunction.
    template <typename TYPE, TYPE VALUE>
    struct constant_t {
        static constexpr TYPE value {VALUE};
    };

    // Bool constants.
    struct true_t  : constant_t<bool, true>  {};
    struct false_t : constant_t<bool, false> {};

/* IS_SAME ################################################################ */
    // Partial specialization: equals types, true.
    template<typename T, typename U>
    struct is_same : false_t {};
    template<typename T>
    struct is_same<T, T> : true_t {};

/* TYPE OF POSITION ######################################################### */
    template<uint8_t pos, typename... TYPES>
    struct type_of_index { static_assert(sizeof...(TYPES) > 0, "type_of_index: there are not types."); };

    // Partial specialization: base case -> position 0.
    template<typename T, typename... TYPES>
    struct type_of_index<0, T, TYPES...> : type_id<T> {};

    // Alias helper: avoid using 'typename' and '::type'.
    template<uint8_t pos, typename... TYPES>
    using type_of_index_t = typename type_of_index<pos, TYPES...>::type;

    // Partial specialization: other position, then recursive search.
    template<uint8_t pos, typename T, typename... TYPES>
    struct type_of_index<pos, T, TYPES...> : type_id<type_of_index_t<pos-1, TYPES...>> {};

/* INDEX OF TYPE ######################################################### */
    template<typename TYPE, typename... TYPES>
    struct index_of_type { static_assert(sizeof...(TYPES) > 0, "Parameter pack with 0 types"); }; 

    // Partial specialization: base case -> first type.
    template<typename T, typename... TYPES> // BASE CASE: when type is match, next index advance is 0.
    struct index_of_type<T, T, TYPES...> : constant_t<std::size_t, 0>{};

    // Alias helper: avoid using '::value'.
    template<typename T, typename...  TYPES>
    static constexpr auto index_of_type_v = index_of_type<T, TYPES...>::value;

    // Partial specialization: base case -> type in the mid.
    template<typename T, typename U, typename... TYPES> // OTHERWISE: index advance in 1.
    struct index_of_type<T, U, TYPES...> : constant_t<std::size_t, index_of_type_v<T, TYPES...> + 1>{}; // (((((0)+1)+1)+1)+1)
    
/*  IF_ELSE ######################################################### */
    template<bool, typename T, typename U>
    struct if_else : type_id<T>{};

    // Partial specialization: case false.
    template<typename T, typename U>
    struct if_else<false, T, U> : type_id<U>{};

    // Alias helper: avoid 'typename' and '::type'
    template<bool CDT, typename T, typename U> 
    using if_else_t = typename if_else<CDT, T, U>::type;

/* REPLACE_CONTAINER ######################################################### */
    template<typename TYPE, template<typename...> class NEW_CONTAINER> // PACK_TYPES = Pack_t<cmp1, cmp2, cmp3..> to CONTAINER<cmp1, cmp2, cmp3, ...>
    struct replace_container {};

    // Partial specialization: first param must be template<Types...>.
    template<template<typename...> class CURREN_CONTAINER, typename... TYPES, template<typename...> class NEW_CONTAINER>
    struct replace_container<CURREN_CONTAINER<TYPES...>, NEW_CONTAINER> : type_id<NEW_CONTAINER<TYPES...>> {};

    // Alias helper: avoid 'typename' and '::type'.
    template<typename TYPE, template<typename...> class NEW_CONTAINER>
    using replace_container_t = typename replace_container<TYPE, NEW_CONTAINER>::type;

/* FOR_ALL_WRAP ######################################################### */
    template<typename TYPE, template<typename...> class NEW_CONTAINER>
    struct for_all_wrap {};

    // Partial specialization: first param must be template<Types...>.
    template<template<typename...> class CURRENT_CONTAINER, typename... TYPES, template<typename...> class NEW_CONTAINER>
    struct for_all_wrap<CURRENT_CONTAINER<TYPES...>, NEW_CONTAINER> : type_id<CURRENT_CONTAINER<NEW_CONTAINER<TYPES>...>> {};

    // Alias helper: avoid 'typename' and '::type'.
    template<typename TYPE, template<typename...> class NEW_CONTAINER>
    using for_all_wrap_t = typename for_all_wrap<TYPE, NEW_CONTAINER>::type;

/* MASK_TYPE ######################################################### */
    template<uint8_t N> // Template type metafunction to get mask type according of number of types.
    struct mask_type_cfg {

        static constexpr std::size_t MAX_VALUE { sizeof(uint64_t) * 8 };
        static_assert( N <= MAX_VALUE, "mask_type_cfg: Parameter pack capacity out of range (max 64)" );

        using type = 
            if_else_t<N <= (8 * sizeof(uint8_t)),  uint8_t, 
            if_else_t<N <= (8 * sizeof(uint16_t)), uint16_t, 
            if_else_t<N <= (8 * sizeof(uint32_t)), uint32_t, uint64_t>>>;
    };
    // Alias helper: avoid 'typename' and '::type'
    template<uint8_t N>
    using mask_type_cfg_t = typename mask_type_cfg<N>::type;

    } // namespace METAPROG


template<typename... TYPES> // Pack of some types.
struct Pack_t {

    static consteval uint8_t size() noexcept {  return sizeof...(TYPES); }

    template<typename TYPE>
    static consteval bool has() noexcept { return (false || ... || METAPROG::is_same<TYPE, TYPES>::value); }

    template<typename TYPE>
    static consteval uint8_t id() noexcept { 
        static_assert(has<TYPE>(), "Pack_t::id: type not found.");
        return METAPROG::index_of_type_v<TYPE, TYPES...>; 
    }

    template<uint8_t id>
    using type_of_id = METAPROG::type_of_index_t<id, TYPES...>;  // Template type metafunction.

    // Choose mask type according of number of types.
    using mask_t = METAPROG::mask_type_cfg_t<size()>;

    template<typename... Ts> // cmp1 = 1 ; cmp2 = 2   =>   00000001 << 1 | 00000001 << 2  =  00000010 | 00000100 = 00000011.
    static consteval mask_t mask() noexcept { return ( 0 | ... | (1 << id<Ts>()) ); }
};

} // namespace UVENGINE