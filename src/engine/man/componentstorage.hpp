#pragma once
#include <engine/util/typepack.hpp>
#include  <engine/util/slotmap.hpp>

namespace uvengine {


template<typename CMPPACK, typename TAGPACK, std::size_t CAPACITY>
struct ComponentStorage_t { /*static_assert(false, "ComponentStorage_t: TypePack<ts..> parameter requeried.");*/ };
//
//
//// Partial specialization: TypePack_t<Ts...> parameters requeried.
template<typename... Cmps, typename... Tags, std::size_t CAPACITY>
struct ComponentStorage_t<TypePack_t<Cmps...>, TypePack_t<Tags...>, CAPACITY> {
//template<typename CMPPACK, typename TAGPACK, std::size_t CAPACITY=100>
//struct ComponentStorage_t {
private:
    // Wrapper to PACK = TypPack_t<Ts...>
    template<typename PACK>
    struct PackTraits_t {

        template<uint8_t id>
        using type_of_pos = typename PACK::template type_of_pos<id>; // Template type metafunction.
        using mask_t      = typename PACK::mask_t;

                                    static consteval uint8_t size()  noexcept { return PACK::size();                    }
        template<typename Type>     static consteval bool    has()   noexcept { return PACK::template has<Type>();      }
        template<typename Type>     static consteval uint8_t id()    noexcept { return PACK::template id<Type>();       }
        template<typename... Types> static consteval mask_t  mask()  noexcept { return PACK::template mask<Types...>(); }
    };

public:

    using CMPPACK = TypePack_t<Cmps...>;
    using TAGPACK = TypePack_t<Tags...>;
    using tag_cfg = PackTraits_t<TAGPACK>; // TAGPACK = TypePack_t<Types...>
    using cmp_cfg = PackTraits_t<CMPPACK>; // CMPPACK = TypePack_t<Types...>

    template<typename Type>
    using slotmap_t     = Slotmap_t<Type, CAPACITY>;
    using pack_to_tuple = metaprog::replace_container_t<CMPPACK, std::tuple>;  // TypePack_t<Types...> to std::tuple<Types...>
    using container_t   = metaprog::for_all_wrap_t<pack_to_tuple, slotmap_t>;  // std::tuple<Types...> to std::tuple<SlotMap_t<Types>...>


    template<typename Cmp>
    [[nodiscard]] slotmap_t<Cmp> const& getContainer() const noexcept
    {
        static_assert(cmp_cfg::template has<Cmp>(), "ComponentStorage_t::getContainer: cmp not found.");
        return std::get<slotmap_t<Cmp>>(_components);
    }

    template<typename Cmp>
    [[nodiscard]] slotmap_t<Cmp>& getContainer() noexcept
    {
        static_assert(cmp_cfg::template has<Cmp>(), "ComponentStorage_t::getContainer: cmp not found.");
        return std::get<slotmap_t<Cmp>>(_components);
    }

private:
    container_t _components{}; // std::tuple<SlotMap_t<Types>...>
};

    

} // namespace uvengine