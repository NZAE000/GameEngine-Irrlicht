#pragma once
#include<game/util/typealiases.hpp>
#include<engine/util/gframe.hpp>

namespace GAME {

struct RenderSys_t {

    explicit RenderSys_t() = default;

    void update(UVENGINE::EManager_t& Eman, UVENGINE::GFrameDevice_t& gfx)
    {
        Eman.forAll([&Eman](UVENGINE::Entity_t& entity)
        {
            auto& rencmp = Eman.getComponent<GAME::RenderCmp_t>(entity);
            auto& phycmp = Eman.getComponent<GAME::PhysicsCmp_t>(entity);

            rencmp.node->setPosition(irr::core::vector3df{phycmp.x, phycmp.y, phycmp.z});
            //e.rencmp.node->getPosition();
        });

        gfx.beginScene();
        gfx.drawAll();
        gfx.endScene();
    }
};
    
} // namespace GAME
