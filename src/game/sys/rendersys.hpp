#pragma once
#include<game/util/typealiases.hpp>
#include<game/util/gframe.hpp>

namespace GAME {

struct RenderSys_t {

    using SYS_CMPS = UVENGINE::CmpPack_t<GAME::RenderCmp_t, GAME::PhysicsCmp_t>;
    using SYS_TAGS = UVENGINE::TagPack_t<>;

    explicit RenderSys_t() = default;

    void update(UVENGINE::EManager_t& Eman, GAME::GFrameDevice_t& gfx)
    {

        Eman.forEach<SYS_CMPS, SYS_TAGS>
        ([](UVENGINE::Entity_t& entity, GAME::RenderCmp_t& rencmp, GAME::PhysicsCmp_t const& phycmp)
        {
            rencmp.node->setPosition(irr::core::vector3df{phycmp.x, phycmp.y, phycmp.z});
        });

        gfx.beginScene();
        gfx.drawAll();
        gfx.endScene();

        //Eman.forEach<GAME::RenderCmp_t, GAME::PhysicsCmp_t>
        //([](UVENGINE::Entity_t& entity, GAME::RenderCmp_t& rencmp, GAME::PhysicsCmp_t const& phycmp)
        //{
        //    rencmp.node->setPosition(irr::core::vector3df{phycmp.x, phycmp.y, phycmp.z});
        //});

        //Eman.forAll([&Eman](UVENGINE::Entity_t& entity)
        //{
        //    auto&       rencmp = Eman.getComponent<GAME::RenderCmp_t>(entity);
        //    auto const& phycmp = Eman.getComponent<GAME::PhysicsCmp_t>(entity);

        //    rencmp.node->setPosition(irr::core::vector3df{phycmp.x, phycmp.y, phycmp.z});
        //    //e.rencmp.node->getPosition();
        //});
    }
};
    
} // namespace GAME
