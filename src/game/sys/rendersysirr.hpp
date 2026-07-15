#pragma once
#include<game/util/typealiases.hpp>
#include<game/util/irrinterface.hpp>

namespace game {

struct RenderSysIrr_t {

    using REQUIRED_CMPS = uvengine::TypePack_t<game::RenderCmpIrr_t, game::PhysicsCmp_t>;
    using REQUIRED_TAGS = uvengine::TypePack_t<>;

    explicit RenderSysIrr_t() = default;

    void update(uvengcfg::EManager_t& Eman, game::irrinterface::GFXDevice_t& gfx)
    {

        Eman.forEach<REQUIRED_CMPS, REQUIRED_TAGS>(
        [](game::RenderCmpIrr_t& rencmp, game::PhysicsCmp_t const& phycmp)
        {
            rencmp.node->setPosition(irr::core::vector3df{phycmp._x, phycmp._y, phycmp._z});
        });

        //Eman.forEach<game::RenderCmp_t, game::PhysicsCmp_t>
        //([](uvengcfg::Entity_t& entity, game::RenderCmp_t& rencmp, game::PhysicsCmp_t const& phycmp)
        //{
        //    rencmp.node->setPosition(irr::core::vector3df{phycmp.x, phycmp.y, phycmp.z});
        //});

        //Eman.forAll([&Eman](uvengcfg::Entity_t& entity)
        //{
        //    auto&       rencmp = Eman.getComponent<game::RenderCmp_t>(entity);
        //    auto const& phycmp = Eman.getComponent<game::PhysicsCmp_t>(entity);
        //    rencmp.node->setPosition(irr::core::vector3df{phycmp.x, phycmp.y, phycmp.z});
        //    //e.rencmp.node->getPosition();
        //});

        gfx.beginScene();
        gfx.drawAll();
        gfx.endScene();
    }
};
    
} // namespace game
