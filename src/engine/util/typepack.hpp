#include<cstdint>
#include<tuple>

namespace uvengine {

    namespace metaprog {

    // Template value metafunction.
    template<typename T, T v>
    struct constant_t {
        static constexpr T value {v};
    };
    // Template type metafunction.
    template<typename T>
    struct type_of { using type = T; };

/* IS_SAME ################################################################ */
    template<typename T, typename U>
    struct is_same : constant_t<bool, false> {};

    // Partial specialization: equals types, true
    template<typename T>
    struct is_same<T, T> : constant_t<bool, true> {};

    // Alias helper
    template<typename T, typename U>
    static constexpr auto is_same_v = is_same<T, U>::value;

/* TYPE OF POSITION ######################################################### */
    template<uint8_t pos, typename... Types>
    struct type_of_position { static_assert(sizeof...(Types) > 0, "type_of_position: there are not types."); };

    // Partial specialization: base case -> position 0.
    template<typename T, typename... Types>
    struct type_of_position<0, T, Types...> : type_of<T> {};

    // Alias helper: avoid using 'typename' and '::type'.
    template<uint8_t pos, typename... Types>
    using type_of_position_t = typename type_of_position<pos, Types...>::type;

    // Partial specialization: other position, then recursive search.
    template<uint8_t pos, typename T, typename... Types>
    struct type_of_position<pos, T, Types...> : type_of<type_of_position_t<pos-1, Types...>> {};

/* POSITION OF TYPE ######################################################### */
    template<typename T, typename...  Types>
    struct position_of_type { static_assert(sizeof...(Types) > 0, "position_of_type: there are not types."); };

    // Partial specialization: base case -> first type.
    template<typename T, typename...  Types>
    struct position_of_type<T, T, Types...> : constant_t<uint8_t, 0> {};

    // Alias helper: avoid using '::value'.
    template<typename T, typename...  Types>
    static constexpr auto position_of_type_v = position_of_type<T, Types...>::value;

    // Partial specialization: base case -> type in the mid.
    template<typename T, typename U, typename...  Types>
    struct position_of_type<T, U, Types...> : constant_t<uint8_t, position_of_type_v<T, Types...> + 1> {};

/* IF_ELSE ######################################################### */
    template<bool condition, typename T1, typename T2>
    struct if_else : type_of<T1> {}; 
    // Partial specialization: case false.
    template<typename T1, typename T2>
    struct if_else<false, T1, T2> : type_of<T2> {};
    // Alias helper
    template<bool condition, typename T1, typename T2>
    using if_else_t = typename if_else<condition, T1, T2>::type;

/* REPLACE_CONTAINER ######################################################### */
    template<typename Type, template<typename...> class new_container>
    struct replace_container {};

    // Partial specialization: first param must be template<Types...>.
    template<template<typename...> class current_container, typename... Types, template<typename...> class new_container> 
    struct replace_container<current_container<Types...>, new_container> : type_of<new_container<Types...>>{};

    // Alias helper: avoid 'typename' and '::type'.
    template<typename Type, template<typename...> class new_container>
    using replace_container_t = typename replace_container<Type, new_container>::type;

/* FOR_ALL_WRAP ######################################################### */
    template<typename Type, template<typename...> class new_container>
    struct for_all_wrap {};
    
    // Partial specialization: first param must be template<Types...>.
    template<template<typename...> class current_container, typename... Types, template<typename...> class new_container>
    struct for_all_wrap<current_container<Types...>, new_container> : type_of<std::tuple<new_container<Types>...>>{};

    // Alias helper: avoid 'typename' and '::type'.
    template<typename Type, template<typename...> class container>
    using for_all_wrap_t = typename for_all_wrap<Type, container>::type;

/* MASK_TYPE ######################################################### */
    template<uint8_t N>  // Choose mask type according of number of types.
    struct mask_type {
        static_assert(N <= 64, "TypePack_t: outside the type limit (64).");  
        using type = metaprog::if_else_t<N <= 8,  uint8_t, 
                     metaprog::if_else_t<N <= 16, uint16_t, 
                     metaprog::if_else_t<N <= 32, uint32_t, uint64_t>>>;
    };
    // Alias helper: avoid 'typename' 'and ::type';
    template<uint8_t N>
    using mask_type_t = typename mask_type<N>::type;

} // namespace metaprog


template<typename... Types> // Types: components, tags, etc.
struct TypePack_t {

    template<uint8_t id>
    using type_of_pos = metaprog::type_of_position_t<id, Types...>;

    // Know how manny types there are.
    static consteval uint8_t size() noexcept { return sizeof...(Types); }

    // Know if a type belong to pack.
    template<typename Type>
    static consteval bool has() noexcept {
        return (false || ... || metaprog::is_same<Type, Types>::value);
    }

    // Get type id.
    template<typename Type>
    static consteval uint8_t id() noexcept 
    { 
        static_assert(has<Type>(), "TypePack_t::id: type not found.");
        return metaprog::position_of_type_v<Type, Types...>;
    }  

    // Choose mask type according of number of types.
    using mask_t = metaprog::mask_type_t<size()>;

    // Mask of type/s.
    template<typename... Ts> // No types? return 0.
    static consteval mask_t mask() noexcept {
        static_assert((true && ... && has<Ts>()), "TypePack_t::mask: type not found.");
        //static_assert(sizeof...(Ts) > 0, "TypePack_t::mask: there are not types.");
        return (0 | ... | (1 << id<Ts>()) ); // ( 1 = 0b00000001 | 2 = 0b00000010 | 32 = 0b00100000) = 0b00100011
    }
};

} // namespace uvengine