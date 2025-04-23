#include<iostream>
#include<stdexcept>
#include<memory>
#include<cstdint>
#include<cassert>
#include<type_traits>
#include<game/util/gframe.hpp>
#include<game/util/typealiases.hpp>
#include<game/sys/rendersys.hpp>
#include<game/sys/physicssys.hpp>
#include<engine/util/vector3.hpp>
//#define NDEBUG


void compile_time_verifications()
{
// Verify cmps and tags
    static_assert( UVENGINE::CmpStorage_t::cmp_cfg::size() == 3 );
    static_assert( UVENGINE::CmpStorage_t::tag_cfg::size() == 4 );
    static_assert( UVENGINE::CmpStorage_t::cmp_cfg::has<GAME::AICmp_t>() );
    static_assert( UVENGINE::CmpStorage_t::tag_cfg::id<GAME::TGBullet_t>() == 2 );
    static_assert( UVENGINE::CmpStorage_t::cmp_cfg::mask<GAME::RenderCmp_t, GAME::PhysicsCmp_t>() == 5, "bad mask" );
    static_assert( std::is_same<UVENGINE::CmpStorage_t::tag_cfg::mask_t, uint8_t>::value == true );

// Verify vector 3d operators
    using Vect3f_t = UVENGINE::Vect3_t<float>;
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

    
// See types
    seetype(UVENGINE::CmpStorage_t::tag_cfg::mask_t{});
    seetype(UVENGINE::CmpStorage_t::pack_to_tuple{});
    seetype(UVENGINE::CmpStorage_t::Storage_t{});
}

int 
main(void){
try {

    compile_time_verifications();
    

    UVENGINE::EManager_t EntityMan {};
    GAME::GFrameDevice_t IrrDevice {800, 600};
    GAME::RenderSys_t  RenderSys{};
    GAME::PhysicsSys_t PhysicsSys{};

    auto& ent1 { EntityMan.createEntity() };
    auto& phycmp = EntityMan.addComponent<GAME::PhysicsCmp_t>(ent1, GAME::PhysicsCmp_t{.z=-10.0f, .vz=.005f});
    EntityMan.addComponent<GAME::RenderCmp_t>(ent1, IrrDevice.createSphere("/Users/eliezerzuniga/Documents/progra/c++/irrlicht/GameEngine-Irrlicht/media/wall.bmp"));

    [[maybe_unused]] auto const& rencmp = EntityMan.getComponent<GAME::RenderCmp_t>(ent1);

    IrrDevice.addStaticText();
    while(IrrDevice.run())
    {
        RenderSys.update(EntityMan, IrrDevice);
        PhysicsSys.update(EntityMan);
    }

    std::cout<<"x: "<<phycmp.x<<" y: "<<phycmp.y<<" z: "<<phycmp.z<<'\n';
    

    //std::cout<<cmpStorage.getMask<GAME::PhysicsCmp_t>()<<'\n';
    //std::cout<<cmpStorage.getMask<GAME::RenderCmp_t>()<<'\n';
    //std::cout<<cmpStorage.getMask<GAME::AICmp_t>()<<'\n';
//
    //auto& phyContainer { cmpStorage.getContainer<GAME::PhysicsCmp_t>() };
    //auto key = phyContainer.insert(GAME::PhysicsCmp_t{0,0,0,1,1,1});
//
    //for (auto& phycmp : phyContainer){
    //    std::cout<<phycmp.vx<<phycmp.vy<<phycmp.vy<<'\n';
    //}
    
    //GAME::PhysicsSys_t PhySys;
    //GAME::RenderSys_t  RenSys;
//
    //UVENGINE::GFrameDevice_t device{640, 360};
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