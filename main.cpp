#include<iostream>
#include<stdexcept>
#include<irrlicht/irrlicht.h>
#include<memory>
#include<vector>
#include<cstdint>


struct GFrameDevice_t {

    using u32 = irr::u32;

// Constructor
    explicit GFrameDevice_t(u32 w, u32 h)
    : width_{w}, height_{h}
    {   
        irr::IrrlichtDevice* device { device_.get() };
        if (!device) throw std::runtime_error("Can't initialize irrlicht device"); // Check!!

        device->setWindowCaption(L"window - Irrlicht demo");
        sceneMan_->addCameraSceneNodeFPS();
    }

// Methods
    void addStaticText(){
        guiEnv_->addStaticText(L"Hello world!!", irr::core::rect<irr::s32>(10, 10, 260, 22), true);
    }

    bool run() const  { return device_->run();      }
    void beginScene() { videoDriver_->beginScene(true, true, irr::video::SColor(255,100,101,140)); } // The beginScene call clears the screen with a color and the depth buffer
    void endScene()   { videoDriver_->endScene();   }
    void drawAll()
    { 
        sceneMan_->drawAll();
        guiEnv_->drawAll();
    }

    irr::scene::ISceneNode* createSphere()
    {
        irr::scene::ISceneNode* node = sceneMan_->addSphereSceneNode();
        if (!node) throw std::runtime_error("Couldn't create sphere");

        auto* texture { videoDriver_->getTexture("media/wall.bmp") };
        if (!texture) throw std::runtime_error("Couldn't create texture");

        node->setPosition(irr::core::vector3df(0, 0, 0));
        node->setMaterialTexture(0, texture);
        node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        
        return node;
    }

private:

// Widow dimension
    u32 width_{}, height_{};

// Resource destroyer
    inline static void destroyDevice(irr::IrrlichtDevice* dv) { dv->drop(); }

    // unique_ptr check if the pointer isn't null to call destructor.
    std::unique_ptr<irr::IrrlichtDevice, void(*)(irr::IrrlichtDevice*)> /*<type, destroyer>*/
    device_ {
        irr::createDevice(
                irr::video::EDT_SOFTWARE,                           // deviceType: This can currently be the Null-device, one of the two software renderers, D3D8, D3D9, or OpenGL. In this example we use EDT_SOFTWARE, but to try out, you might want to change it to EDT_BURNINGSVIDEO, EDT_NULL, EDT_DIRECT3D8, EDT_DIRECT3D9, or EDT_OPENGL.
                irr::core::dimension2d<irr::u32>(width_, height_),  // windowSize: Size of the Window or screen in FullScreenMode to be created
                16,         // bits_per_pixel: Amount of color bits per pixel. This should be 16 or 32. The parameter is often ignored when running in windowed mode.
                false,      // fullscreen:     Specifies if we want the device to run in fullscreen mode or not.
                false,      // stencilbuffer:  Specifies if we want to use the stencil buffer (for drawing shadows).
                false,      // vsync:          Specifies if we want to have vsync enabled, this is only useful in fullscreen mode
                0)          // eventReceiver:  An object to receive events. We do not want to use this parameter here, and set it to 
        ,   destroyDevice
    };

// Pointers const(non-modifiable address): to the VideoDriver, the SceneManager and the graphical user interface environment.
    irr::video::IVideoDriver * const videoDriver_ { device_.get()? device_->getVideoDriver()    : nullptr };
    irr::scene::ISceneManager* const sceneMan_    { device_.get()? device_->getSceneManager()   : nullptr };
    irr::gui::IGUIEnvironment* const guiEnv_      { device_.get()? device_->getGUIEnvironment() : nullptr };
};


namespace ENGINE {

// Manager
template<typename TYPE>
struct EntityManager_t {

    static constexpr std::size_t DEFAULT_INITIAL_ENTITIES {100};

    explicit EntityManager_t(const std::size_t size=DEFAULT_INITIAL_ENTITIES)
    {
        entities_.reserve(size);
    }

    TYPE& createEntity() { return entities_.emplace_back(); }

    template<typename CALLABLE>
    void forAll(CALLABLE&& process)
    {
        std::for_each(begin(entities_), end(entities_), process);
    }

private:
    std::vector<TYPE> entities_;
};

} // namespace ECS


namespace GAME {

// Components
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

    void update(ENGINE::EntityManager_t<Entity_t>& man)
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

    void update(ENGINE::EntityManager_t<Entity_t>& man, GFrameDevice_t& gfx)
    {
        man.forAll([&man](Entity_t& e)
        {
            e.rencmp.node->setPosition(irr::core::vector3df{e.phycmp.x, e.phycmp.y, e.phycmp.z});
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

    ENGINE::EntityManager_t<GAME::Entity_t> EntMan{10};
    GAME::PhysicsSys_t PhySys;
    GAME::RenderSys_t  RenSys;

    GFrameDevice_t device{640, 360};
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