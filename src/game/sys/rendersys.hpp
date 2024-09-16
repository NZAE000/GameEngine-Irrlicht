#pragma once
#include<game/util/typealiases.hpp>
#include<engine/util/gframe.hpp>

namespace GAME {

struct RenderSys_t {

    explicit RenderSys_t() = default;

    void update(UVENGINE::EManager_t& man, UVENGINE::GFrameDevice_t& gfx)
    {
        man.forAll([&man](UVENGINE::Entity_t& e)
        {
            
            //e.rencmp.node->setPosition(irr::core::vector3df{e.phycmp.x, e.phycmp.y, e.phycmp.z});
            //e.rencmp.node->getPosition();
        });

        gfx.beginScene();
        gfx.drawAll();
        gfx.endScene();
    }
};
    
} // namespace GAME
