#include<iostream>
#include<stdexcept>
#include<irrlicht/irrlicht.h>
#include<memory>
#include<vector>
//#include<cstdlib>


struct GameEngine {

    using u32 = irr::u32;

// Constructor
    explicit GameEngine(u32 w, u32 h)
    : width_{w}, height_{h} 
    {   
        irr::IrrlichtDevice* device { device_.get() };
        if (!device) throw std::runtime_error("Can't initialize irrlicht device");

        device->setWindowCaption(L"window - IrrEngine");
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

private:

// Widow dimension
    u32 width_{}, height_{};

// Resource destroyer
    inline static void destroyDevice(irr::IrrlichtDevice* dv) { dv->drop(); }

    // unique_ptr check if the pointer isn't null to call destructor.
    std::unique_ptr<irr::IrrlichtDevice, void(*)(irr::IrrlichtDevice*)> /*<type, destroyer>*/
    device_ {
        irr::createDevice(
                irr::video::EDT_SOFTWARE,                           // deviceType
                irr::core::dimension2d<irr::u32>(width_, height_),  // windowSize
                16,         // bits_per_pixel
                false,      // fullscreen
                false,      // stencilbuffer
                false,      // vsync
                0)          // eventReceiver
        ,   destroyDevice
    };

// Pointers const(non-modifiable address): to the VideoDriver, the SceneManager and the graphical user interface environment
    irr::video::IVideoDriver * const videoDriver_ { device_.get()? device_->getVideoDriver()    : nullptr };
    irr::scene::ISceneManager* const sceneMan_    { device_.get()? device_->getSceneManager()   : nullptr };
    irr::gui::IGUIEnvironment* const guiEnv_      { device_.get()? device_->getGUIEnvironment() : nullptr };
};


struct Entity_t {

    char c_ {'@'};

private:
    std::size_t id_ { ++NEXT_EID };
    inline static std::size_t NEXT_EID;
};

struct EntityManager_t {

    static constexpr std::size_t DEFAULT_INITIAL_ENTITIES {100};

    explicit EntityManager_t(const std::size_t size=DEFAULT_INITIAL_ENTITIES)
    {
        entities_.reserve(size);
    }

    Entity_t& createEntity() { return entities_.emplace_back(); }

private:
    std::vector<Entity_t> entities_;
};


int main(void){ //int argc, const char** argv)
try {

    EntityManager_t entMan{10};
    auto& entity = entMan.createEntity();


    GameEngine device{640, 360};
    device.addStaticText();

    while (device.run())
    {
        device.beginScene();
        device.drawAll();
        device.endScene();
    }
    
    //int a = std::atoi(argv[1]);  //while (a--){ auto* n = new long long; }

    return 0;

} catch(std::exception const& e){
    std::cerr<<"[[Excp]]: " << e.what() <<std::endl;
}

} // main