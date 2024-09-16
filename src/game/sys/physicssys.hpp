#pragma once
#include<engine/man/entitymanager.hpp>
#include<game/util/typealiases.hpp>

namespace GAME {

struct PhysicsSys_t {

    using SYS_CMPS = UVENGINE::CmpPack_t<GAME::PhysicsCmp_t>;
    using SYS_TAGS = UVENGINE::TagPack_t<>;

    explicit PhysicsSys_t() = default;

    void update(UVENGINE::EManager_t& Eman)
    { 
        Eman.forEach<SYS_CMPS, SYS_TAGS>([](UVENGINE::Entity_t& entity, GAME::PhysicsCmp_t& phycmp)
        {
            phycmp.x += phycmp.vx;
            phycmp.y += phycmp.vy;
            phycmp.z += phycmp.vz;
        }); 

        //Eman.forEach<GAME::PhysicsCmp_t>([](UVENGINE::Entity_t& entity, GAME::PhysicsCmp_t& phycmp)
        //{
        //    phycmp.x += phycmp.vx;
        //    phycmp.y += phycmp.vy;
        //    phycmp.z += phycmp.vz;
        //});
        
        //Eman.forAll([&Eman](UVENGINE::Entity_t& entity)
        //{
        //    auto& phycmp = Eman.getComponent<GAME::PhysicsCmp_t>(entity);
        //    phycmp.x += phycmp.vx;
        //    phycmp.y += phycmp.vy;
        //    phycmp.z += phycmp.vz;
        //});
        
    }
};

} // namespace GAME
