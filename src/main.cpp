#include<iostream>
#include<stdexcept>
#include<memory>
#include<cstdint>
#include<type_traits>
#include<game/util/typealiases.hpp>
#include<game/sys/rendersys.hpp>
#include<game/sys/physicssys.hpp>
#include<game/util/irrinterface.hpp>
#include<engine/util/vector3.hpp>
//#define NDEBUG
#include<cassert>


void compile_time_verifications()
{
// Verify cmps and tags
    static_assert(uvengcfg::TAG_PACK::size() == 6,                                      "That is not the size.");
    static_assert(uvengcfg::TAG_PACK::has<game::TGSuperKick>() == true,                 "That type is not there.");
    static_assert(std::is_same_v<uvengcfg::TAG_PACK::type_of_pos<1>, game::TGEnemy>,    "That type is not in that position.");
    static_assert(uvengcfg::TAG_PACK::id<game::TGEnemy>() == 1,                         "That type is not in that position.");
    static_assert(uvengcfg::TAG_PACK::id<game::TGSuperJump>() == 5,                     "That type is not in that position.");
    static_assert(uvengcfg::TAG_PACK::mask<game::TGPlayer>() == 0b01,                   "That type is not in that position.");
    static_assert(uvengcfg::TAG_PACK::mask<game::TGEnemy>() == 0b10,                    "That type is not in that position.");
    static_assert(uvengcfg::TAG_PACK::mask<game::TGPlayer, game::TGEnemy>() == 0b11,    "That type is not in that position.");
    static_assert(std::is_same_v<uvengcfg::TAG_PACK::mask_t, uint8_t>,                  "Is not mask type.");

    static_assert(uvengcfg::CmpStorage_t::tag_cfg::size() == 6,                                     "That is not the size.");
    static_assert(uvengcfg::CmpStorage_t::tag_cfg::has<game::TGPlayer>() == true,                   "That type is not there.");
    static_assert(std::is_same_v<uvengcfg::CmpStorage_t::tag_cfg::type_of_pos<1>, game::TGEnemy>,   "That type is not in that position.");
    static_assert(uvengcfg::CmpStorage_t::tag_cfg::id<game::TGEnemy>() == 1,                        "That type is not in that position.");
    static_assert(uvengcfg::CmpStorage_t::tag_cfg::id<game::TGSuperJump>() == 5,                    "That type is not in that position.");
    static_assert(uvengcfg::CmpStorage_t::tag_cfg::mask<game::TGPlayer>() == 0b01,                  "That type is not in that position.");
    static_assert(uvengcfg::CmpStorage_t::tag_cfg::mask<game::TGEnemy>() == 0b10,                   "That type is not in that position.");
    static_assert(uvengcfg::CmpStorage_t::tag_cfg::mask<game::TGPlayer, game::TGEnemy>() == 0b11,   "That type is not in that position.");
    static_assert(std::is_same_v<uvengcfg::CmpStorage_t::tag_cfg::mask_t, uint8_t>,                 "Is not mask type.");

// Verify vector 3d operators
    using Vect3f_t = uvengine::Vect3_t<float>;
    constexpr Vect3f_t a{0,1,1}, b{1,0,0}, c;

    static_assert( (a + b) == Vect3f_t{ 1, 1, 1} );
    static_assert( (a - b) == Vect3f_t{-1, 1, 1} );
    static_assert( (a * b) == 0 );
    static_assert( (b * 2) == Vect3f_t{2, 0, 0} );
    static_assert( (2 * b) == Vect3f_t{2, 0, 0} );

    static_assert( (a < b)  == 1 );
    static_assert( (a != b) == 1 );
    static_assert( (c >= b) == 0 );
    static_assert( (c == Vect3f_t{0,0,0}) == 1 );

    static_assert( a.lengthSqrt() == 2.0f );
    //static_assert( a.length() == std::sqrt(2.0f));

    //static_assert( Vect3f_t{1,0,0}.normalize() == Vect3f_t{1,0,0} );
}

void seetype(auto) { std::cout<<__PRETTY_FUNCTION__<< '\n'; }


void createWorld(uvengcfg::EManager_t& EntMan, game::irrinterface::GFXDevice_t& IrrDevice)
{
    // Set world's terrain and texture.
    auto& terrain = IrrDevice.createTerrain("media/terrain.bmp", "media/ground.bmp");
    irr::core::aabbox3d<irr::f32> terrainBound = terrain.getBoundingBox();

    // Set camera position
    auto& camera { IrrDevice.getCamera() };
    camera.setPosition({0.0f, terrainBound.MaxEdge.Y+20, -terrainBound.MaxEdge.Z/2-50});
    camera.setTarget({0.0f, 0.0f, 0.0f});

    // First entity.
    auto& sphere1 { EntMan.createEntity() };
    [[maybe_unused]] auto& phycmp1 = EntMan.addComponent<game::PhysicsCmp_t>(sphere1, game::PhysicsCmp_t{._x=-10.0f, ._z=-10.0f, ._vy=.005f});
    EntMan.addComponent<game::RenderCmp_t>(sphere1, &IrrDevice.createSphere("media/wall.bmp"));
    //[[maybe_unused]] auto const& rencmp1 = EntMan.getComponent<game::RenderCmp_t>(sphere1);

    // Second entity.
    auto& sphere2 { EntMan.createEntity() };
    [[maybe_unused]] auto& phycmp2 = EntMan.addComponent<game::PhysicsCmp_t>(sphere2, game::PhysicsCmp_t{._x=10.0f, ._z=-10.0f, ._vy=.003f});
    EntMan.addComponent<game::RenderCmp_t>(sphere2, &IrrDevice.createSphere("media/wall.bmp"));
    //std::cout<<"x: "<<phycmp1._x<<" y: "<<phycmp1._y<<" z: "<<phycmp1._z<<'\n';

// Entities to assert test.
    auto& player = EntMan.createEntity();
    EntMan.addComponent<game::PhysicsCmp_t>(player, .0f, .0f, .0f);
    EntMan.addComponent<game::PhysicsCmp_t>(player, .0f, .0f, .0f); // Add again.. return the before.

    auto& enemy = EntMan.createEntity();
    EntMan.addComponent<game::PhysicsCmp_t>(enemy, game::PhysicsCmp_t{._x=.0, ._y=.0, ._z=.0});
    EntMan.addComponent<game::AICmp_t>(enemy, player.getId());
    
    // Execution time verification (with debug)
    assert(player.getMaskTag() == 0 && "Player entity: incompatible mask tag");
    player.addTag<game::TGPlayer, game::TGSuperKick>();
    assert((player.hasTag<game::TGPlayer, game::TGSuperKick>()) && "It have not mask");
    player.removeTag<game::TGSuperKick>();
    assert(!player.hasTag<game::TGSuperKick>() && "It have mask");
    assert(enemy.getMaskCmp() == 0b110 && "bad enemy mask");
}

int 
main(void){
try {

    //compile_time_verifications();

    uvengcfg::EManager_t            EntityMan  {};
    game::irrinterface::GFXDevice_t IrrDevice  {800, 600};
    game::RenderSys_t               RenderSys  {};
    game::PhysicsSys_t              PhysicsSys {};

    // See types in execution time.
    seetype(uvengcfg::CmpStorage_t{});
    seetype(uvengcfg::CmpStorage_t::container_t{});
    seetype(uvengcfg::EManager_t::CmpStorage_t{});
    seetype(uvengcfg::EManager_t::CmpStorage_t::container_t{});
    seetype(uvengcfg::EManager_t::Entity_t::keystorage_t{});

    // Set world.
    createWorld(EntityMan, IrrDevice);

    // RUN.
    IrrDevice.addStaticText();
    while(IrrDevice.run())
    {
        RenderSys.update(EntityMan, IrrDevice);
        PhysicsSys.update(EntityMan);
    }

    //std::cout<<cmpStorage.getMask<game::PhysicsCmp_t>()<<'\n';
    //std::cout<<cmpStorage.getMask<game::RenderCmp_t>()<<'\n';
    //std::cout<<cmpStorage.getMask<game::AICmp_t>()<<'\n';
//
    //auto& phyContainer { cmpStorage.getContainer<game::PhysicsCmp_t>() };
    //auto key = phyContainer.insert(game::PhysicsCmp_t{0,0,0,1,1,1});
//
    //for (auto& phycmp : phyContainer){
    //    std::cout<<phycmp.vx<<phycmp.vy<<phycmp.vy<<'\n';
    //}
    
    //game::PhysicsSys_t PhySys;
    //game::RenderSys_t  RenSys;
//
    //uvengine::GFrameDevice_t device{640, 360};
    //device.addStaticText();
//
    //auto& entity = EntMan.createEntity();
    //entity.rencmp.node = device.createSphere();
    //entity.phycmp.z  = 10.f;
    //entity.phycmp.vz = .02f;
//
    //while (device.run())
    //{
    //    RenSys.update(EntMan, device);
    //    PhySys.update(EntMan);
    //}

    return 0;

} catch(std::exception const& e){
    std::cerr<<"[[Excp]]: " << e.what() <<std::endl;
}

} // main