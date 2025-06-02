#pragma once
#include<cstdint>
#include<vector>
#include "componentstorage.hpp"

namespace uvengine {


template<typename CMPPACK, typename TAGPACK, std::size_t CAPACITY=100>
struct EntityManager_t { static_assert(false, "EntityManager_t: TypePack_t<Ts...> parameters requeried"); };

// Partial specialization: TypePack_t<Ts...> parameters requeried.
template<typename... Cmps, typename... Tags, std::size_t CAPACITY>
struct EntityManager_t<TypePack_t<Cmps...>, TypePack_t<Tags...>, CAPACITY> {

    using CMPPACK = TypePack_t<Cmps...>;
    using TAGPACK = TypePack_t<Tags...>;

    static constexpr std::size_t Capacity { CAPACITY };
    using CmpStorage_t = ComponentStorage_t<CMPPACK, TAGPACK, CAPACITY>;

    // Mask type for components and tags.
    using cmp_mask_t = typename CmpStorage_t::cmp_cfg::mask_t;
    using tag_mask_t = typename CmpStorage_t::tag_cfg::mask_t;

    template<typename Cmp>
    using to_key_t = typename CmpStorage_t::template slotmap_t<Cmp>::key_t;

    // Entity for user ##############################################
    struct Entity_t {
        
        using pack_to_tuple = metaprog::replace_container_t<CMPPACK, std::tuple>;  // TypePack_t<Types...> to std::tuple<Types...>
        using keystorage_t  = metaprog::for_all_wrap_t<pack_to_tuple, to_key_t>;   // std::tuple<Types...> to std::tuple<key_t<Types>...>

        std::size_t getId() const noexcept { return _id; };

        // ADD TAG MASK.
        template<typename... Tgs>
        void addTag() noexcept { _mask_tag |= CmpStorage_t::tag_cfg::template mask<Tgs...>(); }
        template<typename... Tgs>
        void removeTag() noexcept { _mask_tag ^= CmpStorage_t::tag_cfg::template mask<Tgs...>(); }

        // HAS MASK.
        template<typename... Cps>
        bool hasCmp() const noexcept { return _mask_cmp & CmpStorage_t::cmp_cfg::template mask<Cps...>(); }
        template<typename... Tgs>
        bool hasTag() const noexcept { return _mask_tag & CmpStorage_t::tag_cfg::template mask<Tgs...>(); }

        // GET MASK.
        cmp_mask_t getMaskCmp() const noexcept { return _mask_cmp; }
        tag_mask_t getMaskTag() const noexcept { return _mask_tag; }

        friend struct EntityManager_t<CMPPACK, TAGPACK, CAPACITY>; // Only entity manager can access to private members.
    
    // ENTITY MANAGER ACCESS ONLY.
    private:
        inline static std::size_t NEX_ID {0};
        std::size_t  _id {++NEX_ID};
        keystorage_t _cmpkeys{};
        cmp_mask_t   _mask_cmp{};
        tag_mask_t   _mask_tag{};

        template<typename Cmp>
        void addCmp(to_key_t<Cmp>& key) noexcept
        {
            static_assert(CmpStorage_t::cmp_cfg::template has<Cmp>(), "Entity_t::addCmp: cmp not found into cmpcfg."); // Compilation time.
            assert(!hasCmp<Cmp>() && "Entity_t::addCmp: component was already added."); // Execution time.

            _mask_cmp |= CmpStorage_t::cmp_cfg::template mask<Cmp>(); // Update cmp mask.
            std::get<to_key_t<Cmp>>(_cmpkeys) = key;
        }

        // GET CMP KEY.
        template<typename Cmp>
        to_key_t<Cmp> const& getCmpKey() const { 
            assert(hasCmp<Cmp>() && "Entity_t::getCmpKey: cmp key not found.");
            return std::get<to_key_t<Cmp>>(_cmpkeys); // std::get throw exception.
        }
        template<typename Cmp>
        to_key_t<Cmp>& getCmpKey() {
            assert(hasCmp<Cmp>() && "Entity_t::getCmpKey: cmp key not found");
            return std::get<to_key_t<Cmp>>(_cmpkeys); 
        }

    }; // ##############################################################

    explicit EntityManager_t() { _entities.reserve(Capacity); }

    // CREATE ENTITY
    Entity_t& createEntity() noexcept { return _entities.emplace_back(); }

    // ADD COMPONENT.
    template<typename Cmp, typename... Params>
    Cmp& addComponent(Entity_t& entity, Params&&... args) noexcept
    {
        static_assert(CmpStorage_t::cmp_cfg::template has<Cmp>(), "EntityManager_t::addComponent: cmp not found into cmpcfg.");
        Cmp* cmp {nullptr};
        if (!entity.template hasCmp<Cmp>()) cmp = &createComponent<Cmp>(entity, args...);   // First check.
        else                                cmp = &getComponent<Cmp>(entity);               // Is it? Return the same.
        return *cmp;
    }

    // GET COMPONENT.
    template<typename Cmp>
    Cmp const& getComponent(Entity_t const& entity) const noexcept
    {
        auto& container = _storage.template getContainer<Cmp>(); // Slotmap_t, f. eg.
        to_key_t<Cmp> key = entity.template getCmpKey<Cmp>();
        return container[key];
    }
    template<typename Cmp>
    Cmp& getComponent(Entity_t& entity) noexcept {
        Cmp const& cmp = const_cast<EntityManager_t const*>(this)->getComponent<Cmp>(entity);
        return *const_cast<Cmp*>(&cmp);
    }

    // FOR ENTITIES PROCESS ####################################################################
    template<typename Callback>
    void forAll(Callback&& process)
    {
        std::for_each(begin(_entities), end(_entities), process);
    }

    //template<typename... Cps>
    //void forEach(auto&& process)
    //{   
    //    //cmp_mask_t mask_cmps = CmpStorage_t::cmp_cfg::template mask<Cps...>();
    //    for (auto& entity : _entities){
    //        if (entity.template hasCmp<Cps...>()) { // entity.getMaskCmp() & mask_cmps
    //            process(entity, getComponent<Cps>(entity)...);
    //        }
    //    }
    //}

    template<typename CmpPack, typename TagPack>
    void forEach(auto&& process){
        forEach_impl(process, CmpPack{}, TagPack{}); // Tag Dispatch.
    }

    // Partial specialization: TODO!
    //template<typename... Cps, typename... Tgs>
    //void forEach< TypePack_t<Cps...>, TypePack_t<Tgs...> >(auto&& process)
    //{
    //    bool hasCmps{}, hasTags{};
    //    for (auto& entity : _entities)
    //    {
    //        hasCmps = (true && ... && entity.template hasCmp<Cps>()); // Cmps empty? true default.
    //        hasTags = (true && ... && entity.template hasTag<Tgs>()); // Tags empty? true default.
    //        if (hasCmps && hasTags)
    //            process(getComponent<Cps>(entity)...);
    //    }
    //}

private:

    // WITH TAG DISPATCH. 
    template<typename... Cps, typename... Tgs>
    void forEach_impl(auto&& process, TypePack_t<Cps...>, TypePack_t<Tgs...>)
    {
        bool hasCmps{}, hasTags{};
        for (auto& entity : _entities)
        {
            hasCmps = (true && ... && entity.template hasCmp<Cps>()); // Cmps empty? true default.
            hasTags = (true && ... && entity.template hasTag<Tgs>()); // Tags empty? true default.
            if (hasCmps && hasTags)
                process(getComponent<Cps>(entity)...);
        }
    }


    std::vector<Entity_t> _entities{};
    CmpStorage_t          _storage{};

    template<typename Cmp, typename... Params>
    Cmp& createComponent(Entity_t& entity, Params&&... args) noexcept 
    {
        auto& container = _storage.template getContainer<Cmp>(); // Slotmap_t, f. eg.
        to_key_t<Cmp> key = container.insert(Cmp{std::forward<Params>(args)...});
        entity.template addCmp<Cmp>(key);
        return container[key];
    }
};

} // namespace uvengine