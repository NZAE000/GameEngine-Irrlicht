#pragma once


namespace UVENGINE {

    namespace METAPROG {

        /** Template type metafunction **/
        template<typename TYPE>
        struct Type_id { using type = TYPE; };

        /** Template value metafunction **/
        template <typename TYPE, TYPE VALUE>
        struct Constant_t {
            static constexpr TYPE value {VALUE};
        };

        // Bool constants
        struct True_t  : Constant_t<bool, true>  {};
        struct False_t : Constant_t<bool, false> {};


        /** Template specialization to know if two types are equals **/
        template<typename T1, typename T2>
        struct is_same : False_t {};
        template<typename T>
        struct is_same<T, T> : True_t {};


        /** Template specialization to get the index (position) of the type in the type package **/
        template<typename TYPE, typename... TYPES>
        struct index_t { static_assert(sizeof...(TYPES) > 0, "Parameter pack with 0 types"); };

            // find index of cmp3 on CmpPack<cmp1(0), cmp2(1), cmp3(2), cmp4(3)>
            //  
            //  
            //  

        template<typename T, typename... TYPES> // BASE CASE: when type is match, next index advance is 0.
        struct index_t<T, T, TYPES...> : Constant_t<std::size_t, 0>{};

        template<typename T1, typename T2, typename... TYPES> // OTHERWISE: index advance in 1
        struct index_t<T1, T2, TYPES...> : Constant_t<std::size_t, index_t<T1, TYPES...>::value + 1>{};
        

        /**  Template specialization to choose type according condition **/
        template<bool, typename T1, typename T2>
        struct TernaryCdt_t : Type_id<T1>{};

        template<typename T1, typename T2>
        struct TernaryCdt_t<false, T1, T2> : Type_id<T2>{};

        template<bool CDT, typename T1, typename T2> // Helper
        using TernaryCdt_type = typename TernaryCdt_t<CDT, T1, T2>::type;


        /** Template type metafunction to get mask type according pack size **/
        template<typename PACK>
        struct Masktype_cfg {

            static constexpr std::size_t MAX_VALUE { sizeof(uint64_t) * 8 };
            static_assert( PACK::size() <= MAX_VALUE, "Parameter pack capacity out of range (max 64)" );

            using type = 
                TernaryCdt_type<PACK::size() <= (8 * sizeof(uint8_t)),  uint8_t, 
                TernaryCdt_type<PACK::size() <= (8 * sizeof(uint16_t)), uint16_t, 
                TernaryCdt_type<PACK::size() <= (8 * sizeof(uint32_t)), uint32_t, uint64_t>>>;
        };
        

        /** Pack of some types **/
        template<typename... TYPES>
        struct Pack_t {
            static consteval std::size_t size() noexcept {  return sizeof...(TYPES); }

            template<typename TYPE>
            static consteval bool has() noexcept {  return (false || ... || is_same<TYPE, TYPES>::value); }

            template<typename TYPE>
            static consteval std::size_t index() noexcept { return index_t<TYPE, TYPES...>::value; }
        };

        /** Template structure to provide type information **/
        template<typename PACK> // CmpPack_t<cmp1, cmp2, cmp3, ..> or TagPack_t<tag1, tag2, tag3, ..>
        struct Traits_t {

            //static_assert(METAPROG::is_same<PACK, >) CHECK TYPE PACK!!
            using mask_type = typename Masktype_cfg<PACK>::type; // std::size_t

            static consteval std::size_t size() noexcept { return PACK::size(); }
            
            template<typename TYPE>
            static consteval bool has() noexcept { return PACK::template has<TYPE>(); }

            template<typename TYPE>
            static consteval std::size_t id() noexcept 
            { 
                static_assert( has<TYPE>(), "Type not found into Pack types" );
                return PACK::template index<TYPE>(); 
            }

            template<typename... TYPES> // cmp1 = 1 ; cmp2 = 2   =>   00000001 << 1 | 00000001 << 2  =  00000010 | 00000100 = 00000011.
            static consteval mask_type mask() noexcept { return ( 0 | ... | (1 << id<TYPES>()) ); }
        };
    } // namespace METAPROG


/** Components information **/
template<typename CMP_PACK>
struct  CmpTraits_t : METAPROG::Traits_t<CMP_PACK>{};
/** Tags information **/
template<typename TAG_PACK>
struct  TagTraits_t : METAPROG::Traits_t<TAG_PACK>{};


/*+ User types especification **/
template<typename... CMPS>
struct CmpPack_t : METAPROG::Pack_t<CMPS...> {};
template<typename... TAGS>
struct TagPack_t : METAPROG::Pack_t<TAGS...> {};



} // namespace UVENGINE