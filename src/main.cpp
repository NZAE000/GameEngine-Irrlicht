#include<iostream>
#include<stdexcept>
#include<memory>
#include<cstdint>
#include<engine/man/entitymanager.hpp>
#include<engine/util/gframe.hpp>


namespace GAME {

// Components
struct AICmp_t {
    std::size_t eid;
};

struct PhysicsCmp_t {
    float x{}, y{}, z{};
    float vx{}, vy{}, vz{};
};

struct RenderCmp_t {
    irr::scene::ISceneNode* node{nullptr};
};

struct Entity_t {

    PhysicsCmp_t phycmp;
    RenderCmp_t rencmp;

private:
    std::size_t id_{++NEXT_EID};
    inline static std::size_t NEXT_EID;
};


// Systems
struct PhysicsSys_t {

    explicit PhysicsSys_t() = default;

    void update(UVENGINE::EntityManager_t<Entity_t>& man)
    { 
        man.forAll([man](Entity_t& e)
        {
            e.phycmp.x += e.phycmp.vx;
            e.phycmp.y += e.phycmp.vy;
            e.phycmp.z += e.phycmp.vz;
        }); 
    }
};

struct RenderSys_t {

    explicit RenderSys_t() = default;

    void update(UVENGINE::EntityManager_t<Entity_t>& man, UVENGINE::GFrameDevice_t& gfx)
    {
        man.forAll([&man](Entity_t& e)
        {
            e.rencmp.node->setPosition(irr::core::vector3df{e.phycmp.x, e.phycmp.y, e.phycmp.z});
            //e.rencmp.node->getPosition();
        });

        gfx.beginScene();
        gfx.drawAll();
        gfx.endScene();
    }
};

} // namespace GAME


int 
main(void){
try {

    UVENGINE::EntityManager_t<GAME::Entity_t> EntMan{10};
//    ENGINE::ComponentStorage_t<GAME::PhysicsCmp_t, GAME::RenderCmp_t, GAME::AICmp_t, 10> cmpStorage{};
//
//    std::cout<<cmpStorage.getMask<GAME::PhysicsCmp_t>()<<'\n';
//    std::cout<<cmpStorage.getMask<GAME::RenderCmp_t>()<<'\n';
//    std::cout<<cmpStorage.getMask<GAME::AICmp_t>()<<'\n';
//
//    auto& phyContainer { cmpStorage.getContainer<GAME::PhysicsCmp_t>() };
//    auto key = phyContainer.insert(GAME::PhysicsCmp_t{0,0,0,1,1,1});
//
//    for (auto& phycmp : phyContainer){
//        std::cout<<phycmp.vx<<phycmp.vy<<phycmp.vy<<'\n';
//    }
    
    GAME::PhysicsSys_t PhySys;
    GAME::RenderSys_t  RenSys;

    UVENGINE::GFrameDevice_t device{640, 360};
    device.addStaticText();

    auto& entity = EntMan.createEntity();
    entity.rencmp.node = device.createSphere();
    entity.phycmp.z  = 10.f;
    entity.phycmp.vz = .02f;

    while (device.run())
    {
        RenSys.update(EntMan, device);
        PhySys.update(EntMan);
    }

    return 0;

} catch(std::exception const& e){
    std::cerr<<"[[Excp]]: " << e.what() <<std::endl;
}

} // main