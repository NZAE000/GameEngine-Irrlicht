#pragma once
#include<engine/man/entitymanager.hpp>
#include<game/util/typealiases.hpp>

namespace game {

struct PhysicsSys_t {

    using REQUIRED_CMPS = uvengine::TypePack_t<game::PhysicsCmp_t>;
    using REQUIRED_TAGS = uvengine::TypePack_t<>;

    explicit PhysicsSys_t() = default;

    void update(uvengcfg::EManager_t& Eman)
    { 
        Eman.forEach<REQUIRED_CMPS, REQUIRED_TAGS>([](game::PhysicsCmp_t& phycmp)
        {
            phycmp._x += phycmp._vx;
            phycmp._y += phycmp._vy;
            phycmp._z += phycmp._vz;
        }); 

        //Eman.forEach<game::PhysicsCmp_t>([](uvengcfg::Entity_t& entity, game::PhysicsCmp_t& phycmp)
        //{
        //    phycmp.x += phycmp.vx;
        //    phycmp.y += phycmp.vy;
        //    phycmp.z += phycmp.vz;
        //});
        
        //Eman.forAll([&Eman](uvengcfg::Entity_t& entity)
        //{
        //    auto& phycmp = Eman.getComponent<game::PhysicsCmp_t>(entity);
        //    phycmp.x += phycmp.vx;
        //    phycmp.y += phycmp.vy;
        //    phycmp.z += phycmp.vz;
        //});
        
    }
};

} // namespace game
