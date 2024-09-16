#pragma once
#include<vector>
#include "componentstorage.hpp"

namespace UVENGINE {



// Manager: CMP_PACK = CmPack_t<cmp1, cmp2, cmp3, ...>  - TAG_PACK = TagPack_t<tag1, tag2, tag3, ...>
template<typename CMP_PACK, typename TAG_PACK, std::size_t CAPACITY = 100>
struct EntityManager_t {

// Aliases ################################################################
    template<typename CMP>
    using key_type     = typename Slotmap_t<CMP, CAPACITY>::key_type;
    using CmpStorage_t = ComponentStorage_t<CMP_PACK, TAG_PACK, CAPACITY>;

    using Tuple_cmps   = typename METAPROG::Replace_with<CMP_PACK, std::tuple>::type;          // CmpPack_t<cmp1, cmp2, cmp3..> to std::tuple<cmp1, cmp2, cmp3, ...>
    using KeyStorage_t = typename METAPROG::ForAll_types_wrap<Tuple_cmps, key_type>::type;     // std::tuple<cmp1, cmp2, cmp3, ...> to std::tuple<key_type<cmp1>, key_type<cmp2>, key_type<cmp3>, ...>

// ########################################################################

    struct Entity_t { // Entity for the user!! ################################################

        template<typename CMP>
        void addCmp(key_type<CMP> key)
        {
            cmp_mask |= CmpStorage_t::cmp_cfg::template mask<CMP>(); // Set cmp_mask (any bit to 1)
            std::get<key_type<CMP>>(cmpKeys) = key; // Add key
        }

        template<typename TAG>
        void addTag(void) {
            tag_mask |= CmpStorage_t::tag_cfg::template mask<TAG>(); // Set tag_ mask (any bit to 1)
        }

        template<typename CMP>
        bool hasCmp() const noexcept {
            return cmp_mask & CmpStorage_t::cmp_cfg::template mask<CMP>(); // f. ex: (00010110 & 00010000) = 00010000 = true
        }
        template<typename TAG>
        bool hasTag() const noexcept {
            return tag_mask & CmpStorage_t::tag_cfg::template mask<TAG>(); // f. ex: (00010110 & 00010000) = 00010000 = true
        }

        template<typename CMP>
        [[nodiscard]] key_type<CMP> getCmpKey() const {
            return std::get<key_type<CMP>>(cmpKeys);
        }


        typename CmpStorage_t::cmp_cfg::mask_type cmp_mask {};
        typename CmpStorage_t::tag_cfg::mask_type tag_mask {};
        std::size_t id { ++NEXT_ID }; // Each entity created, set next id.

    private:
        inline static std::size_t NEXT_ID {0};
        KeyStorage_t cmpKeys{};

    }; // ########################################################################################


    static constexpr std::size_t DEFAULT_INITIAL_ENTITIES {CAPACITY};

    explicit EntityManager_t() { entities_.reserve(CAPACITY); }

    Entity_t& createEntity() { return entities_.emplace_back(); }


    template<typename CMP, typename... PARAMS>
    CMP& addComponent(Entity_t& entity, PARAMS&&... args)
    {
        if ( entity.template hasCmp<CMP>() ) // Check if the entity has  cmp
            return getComponent<CMP>(entity);

        return createComponent<CMP>(entity, args...);
    }

    template<typename CMP>
    CMP const& getComponent(Entity_t& entity) const
    {
        auto& storage { cmpStorage.template getContainer<CMP>() };
        return storage[entity.template getCmpKey<CMP>()]; // Get cmp data with the key and return;
    }

    template<typename CMP>
    CMP& getComponent(Entity_t& entity) // Get cmp data (read only) with the key and return;
    {
        auto const& cmp { const_cast<EntityManager_t const*>(this)->getComponent<CMP>(entity) };
        return *const_cast<CMP*>(&cmp);
    }


    template<typename CALLABLE>
    void forAll(CALLABLE&& process)
    {
        std::for_each(begin(entities_), end(entities_), process);
    }

    template<typename... CMPS, typename... TAGS> // Update entities with specified components and tags
    void forEach(auto&& process)
    {
        std::for_each(begin(entities_), end(entities_), [&](auto& entity)
        {   
            bool hasCmps = true && ( ... && entity.template hasCmp<CMPS>() );
            bool hasTags = true && ( ... && entity.template hasTag<TAGS>() );

            if ( true && hasCmps && hasTags )
                process(entity, getComponent<CMPS>(entity)...);
        });
    }


private:

    template<typename CMP, typename... PARAMS>
    CMP& createComponent(Entity_t& entity, PARAMS&&... args)
    {
        auto& storage { cmpStorage.template getContainer<CMP>() };
        auto key { storage.insert( CMP{std::forward<PARAMS>(args)...} ) }; // Create new cmp data (std::forward to decide if pass L or R value reference)
        entity.template addCmp<CMP>(key);   // Add key into entity
        return storage[key]; // Get cmp data with the key and return;
    }

    std::vector<Entity_t> entities_{};
    CmpStorage_t cmpStorage{};
};

} // namespace UVENGINE