#include<iostream>
#include<stdexcept>
#include<memory>
#include<cstdint>
#include<cassert>
#include<type_traits>
#include<engine/util/gframe.hpp>
#include<game/util/typealiases.hpp>
#include<game/sys/rendersys.hpp>
#include<game/sys/physicssys.hpp>


int 
main(void){
try {

// Verify compiling time
    static_assert( UVENGINE::CmpStorage_t::cmp_cfg::size() == 3 );
    static_assert( UVENGINE::CmpStorage_t::tag_cfg::size() == 4 );
    static_assert( UVENGINE::CmpStorage_t::cmp_cfg::has<GAME::AICmp_t>() );
    static_assert( UVENGINE::CmpStorage_t::tag_cfg::id<GAME::TGBulle_Tt>() == 2);
    static_assert( UVENGINE::CmpStorage_t::cmp_cfg::mask<GAME::RenderCmp_t, GAME::PhysicsCmp_t>() == 5, "bad mask");
    static_assert( std::is_same<UVENGINE::CmpStorage_t::tag_cfg::mask_type, uint8_t>::value == true );
// See types
    seetype(UVENGINE::CmpStorage_t::tag_cfg::mask_type{});
    seetype(UVENGINE::CmpStorage_t::TupleCmp_t{});
    seetype(UVENGINE::CmpStorage_t::Storage_t{});
    

    UVENGINE::EManager_t EntityMan {};
    UVENGINE::GFrameDevice_t IrrDevice {640, 360};
    GAME::RenderSys_t RenderSys{};
    GAME::PhysicsSys_t PhysicsSys{};

    auto& ent1 { EntityMan.createEntity() };
    auto& phycmp = EntityMan.addComponent<GAME::PhysicsCmp_t>(ent1, GAME::PhysicsCmp_t{.x=1.0f, .y=1.0f, .z=10.0f});
    EntityMan.addComponent<GAME::RenderCmp_t>(ent1, IrrDevice.createSphere());

    auto const& rencmp = EntityMan.getComponent<GAME::RenderCmp_t>(ent1);

    while(IrrDevice.run())
    {
        PhysicsSys.update(EntityMan);
        RenderSys.update(EntityMan, IrrDevice);
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