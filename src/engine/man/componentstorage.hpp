#pragma once
#include<engine/util/slotmap.hpp>
#include<engine/util/packtraits.hpp>
#include<tuple>

namespace UVENGINE {

//using CmpType_t = std::uint8_t;
//
//template<typename... CMPS>
//using Tuple_t = std::tuple<CMPS...>;
    namespace METAPROG {
         
        /** Template specialization to change container for other **/ 
        template<typename PACK_TYPES, template<typename...> class CONTAINER> // PACK_TYPES = CmpPack_t<cmp1, cmp2, cmp3..> to CONTAINER<cmp1, cmp2, cmp3, ...>
        struct Replace_with {};

        template<template<typename...> class CONTAINER, template<typename...> class PACK_T, typename... TYPES>
        struct Replace_with<PACK_T<TYPES...>, CONTAINER> : Type_id<CONTAINER<TYPES...>> {};

        /** Template specialization to wrap each types with some container **/
        template<typename COLLECTION_TYPES, template<typename...> class CONTAINER>
        struct ForAll_types_wrap {};

        template<template<typename...> class CONTAINER, template<typename...> class COLLECTION, typename... TYPES>
        struct ForAll_types_wrap<COLLECTION<TYPES...>, CONTAINER> : Type_id<COLLECTION<CONTAINER<TYPES>...>> {};

    } // namespace METAPROG


//template<typename CMP1, typename CMP2, typename CMP3, std::size_t CAPACITY=100>
template<typename CMP_PACK, typename TAG_PACK, std::size_t CAPACITY=100> // CMP_PACK = CmpPack_t<cmp1, cmp2, cmp3, ...>
struct ComponentStorage_t {

    explicit ComponentStorage_t() = default;

    using cmp_cfg = CmpTraits_t<CMP_PACK>;
    using tag_cfg = TagTraits_t<TAG_PACK>;

    template<typename TYPE>
    using SltMap_t = Slotmap_t<TYPE, CAPACITY>;

    using TupleCmp_t = typename METAPROG::Replace_with<CMP_PACK, std::tuple>::type;          // CmpPack_t<cmp1, cmp2, cmp3..> to std::tuple<cmp1, cmp2, cmp3, ...>
    using Storage_t  = typename METAPROG::ForAll_types_wrap<TupleCmp_t, SltMap_t>::type;     // std::tuple<cmp1, cmp2, cmp3, ...> to std::tuple<Slotmap_t<cmp1, CAPACITY>, Slotmap_t<cmp2, CAPACITY>, Slotmap_t<cmp3, CAPACITY>, ...>

    template<typename CMP>
    Slotmap_t<CMP, CAPACITY>& getContainer() // Get slotmap of some cmp
    {  
        return std::get<Slotmap_t<CMP, CAPACITY>>(components_);
    }

    template<typename CMP>
    Slotmap_t<CMP, CAPACITY> const& getContainer() const // Get slotmap (read only) of some cmp
    {  
        return std::get<Slotmap_t<CMP, CAPACITY>>(components_);
    }

    //template<typename CMP>
    //auto createComponent()
    //{
    //    
    //}

    //template<typename CMP_t>
    //[[nodiscard]] constexpr CmpType_t getCmpId()  const noexcept { return getCmpId(CMP_t{}); }
//
    //template<typename CMP_t>
    //[[nodiscard]] constexpr std::size_t getMask() const noexcept { return 1 << getCmpId<CMP_t>(); }
//
    //template<typename CMP_t> // Get slotmap of CMP_t
    //[[nodiscard]] constexpr Slotmap_t<CMP_t, CAPACITY>& getContainer()
    //{   
    //    return std::get<Slotmap_t<CMP_t, CAPACITY>>(components_);
    //}

private:
// Container of different components (When ComponentStorage_t is instantiated, storage will has all container of all cmps)
    Storage_t components_{};
    //Tuple_t<Slotmap_t<CMP1, CAPACITY>, Slotmap_t<CMP2, CAPACITY>, Slotmap_t<CMP3, CAPACITY>> components_{};
//
    //[[nodiscard]] constexpr CmpType_t getCmpId(CMP1) const noexcept { return 0; }
    //[[nodiscard]] constexpr CmpType_t getCmpId(CMP2) const noexcept { return 1; }
    //[[nodiscard]] constexpr CmpType_t getCmpId(CMP3) const noexcept { return 2; }
};

} // namespace UVENGINE