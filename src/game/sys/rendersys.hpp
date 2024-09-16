#pragma once
#include<game/util/typealiases.hpp>
#include<engine/util/gframe.hpp>

namespace GAME {

struct RenderSys_t {

    using CMP_PACK_USE = UVENGINE::CmpPack_t<GAME::RenderCmp_t, GAME::PhysicsCmp_t>;
    using TAG_PACK_USE = UVENGINE::TagPack_t<>;

    explicit RenderSys_t() = default;

    void update(UVENGINE::EManager_t& Eman, UVENGINE::GFrameDevice_t& gfx)
    {
        //Eman.forAll([&Eman](UVENGINE::Entity_t& entity)
        //{
        //    auto&       rencmp = Eman.getComponent<GAME::RenderCmp_t>(entity);
        //    auto const& phycmp = Eman.getComponent<GAME::PhysicsCmp_t>(entity);
//
        //    rencmp.node->setPosition(irr::core::vector3df{phycmp.x, phycmp.y, phycmp.z});
        //    //e.rencmp.node->getPosition();
        //});
        Eman.forEach<GAME::RenderCmp_t, GAME::PhysicsCmp_t>
        ([](UVENGINE::Entity_t& entity, GAME::RenderCmp_t& rencmp, GAME::PhysicsCmp_t const& phycmp)
        {
            rencmp.node->setPosition(irr::core::vector3df{phycmp.x, phycmp.y, phycmp.z});
        });

        gfx.beginScene();
        gfx.drawAll();
        gfx.endScene();
    }
};
    
} // namespace GAME
