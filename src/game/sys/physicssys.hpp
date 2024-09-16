#pragma once
#include<engine/man/entitymanager.hpp>
#include<game/util/typealiases.hpp>

namespace GAME {

struct PhysicsSys_t {

    explicit PhysicsSys_t() = default;

    void update(UVENGINE::EManager_t& Eman)
    { 
        Eman.forAll([&Eman](UVENGINE::Entity_t& entity)
        {
            auto& phycmp = Eman.getComponent<GAME::PhysicsCmp_t>(entity);
            phycmp.x += phycmp.vx;
            phycmp.y += phycmp.vy;
            phycmp.z += phycmp.vz;
        }); 
    }
};

} // namespace GAME
