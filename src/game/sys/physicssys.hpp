#pragma once
#include<engine/man/entitymanager.hpp>
#include<game/util/typealiases.hpp>

namespace GAME {

struct PhysicsSys_t {

    explicit PhysicsSys_t() = default;

    void update(UVENGINE::EManager_t& man)
    { 
        man.forAll([man](UVENGINE::Entity_t& e)
        {
            //e.phycmp.x += e.phycmp.vx;
            //e.phycmp.y += e.phycmp.vy;
            //e.phycmp.z += e.phycmp.vz;
        }); 
    }
};
} // namespace GAME
