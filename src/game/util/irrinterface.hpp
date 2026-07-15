#pragma once
#include<irrlicht/irrlicht.h>
#include<memory>
#include<stdexcept>
#include<string_view>

namespace game {
    namespace irrinterface {

    // ### GFX Interface ###############################
    struct GFXDevice_t {

        using u32 = irr::u32;

        explicit GFXDevice_t(u32 w, u32 h);

        bool run() const  { return device_->run();      }
        void beginScene() { videoDriver_->beginScene(true, true, irr::video::SColor(255,100,101,140)); } // The beginScene call clears the screen with a color and the depth buffer
        void endScene()   { videoDriver_->endScene();   }
        void drawAll()
        { 
            sceneMan_->drawAll();
            guiEnv_->drawAll();
        }

        void addStaticText(){ guiEnv_->addStaticText(L"Irrlicht game!!", irr::core::rect<irr::s32>(10, 10, 260, 22), true); }
        
        irr::scene::ISceneNode&         createSphere(std::string_view texture_path);
        irr::scene::ITerrainSceneNode&  createTerrain(std::string_view map_path, std::string_view texture_path);    
        irr::scene::ICameraSceneNode&   getCamera()            noexcept { return *camera_; }
        irr::video::IVideoDriver const& getVideoDriver() const noexcept { return *videoDriver_; }

    private:

        // Widow dimension
        u32 width_{}, height_{};

        // Resource destroyer
        inline static void destroyDevice(irr::IrrlichtDevice* dv) { dv->drop(); }

        // unique_ptr check if the pointer isn't null to call destructor.
        std::unique_ptr<irr::IrrlichtDevice, void(*)(irr::IrrlichtDevice*)> /*<type, destroyer>*/
        device_ {
            irr::createDevice(
                    irr::video::EDT_OPENGL,                           // deviceType: This can currently be the Null-device, one of the two software renderers, D3D8, D3D9, or OpenGL. In this example we use EDT_SOFTWARE, but to try out, you might want to change it to EDT_BURNINGSVIDEO, EDT_NULL, EDT_DIRECT3D8, EDT_DIRECT3D9, or EDT_OPENGL.
                    irr::core::dimension2d<irr::u32>(width_, height_),  // windowSize: Size of the Window or screen in FullScreenMode to be created
                    32,         // bits_per_pixel: Amount of color bits per pixel. This should be 16 or 32. The parameter is often ignored when running in windowed mode.
                    false,      // fullscreen:     Specifies if we want the device to run in fullscreen mode or not.
                    false,      // stencilbuffer:  Specifies if we want to use the stencil buffer (for drawing shadows).
                    false,      // vsync:          Specifies if we want to have vsync enabled, this is only useful in fullscreen mode.
                    0)          // eventReceiver:  An object to receive events. We do not want to use this parameter here, and set it to 0.
            ,   destroyDevice
        };

        // Pointers const(non-modifiable address): to the VideoDriver, the SceneManager and the graphical user interface environment.
        irr::video::IVideoDriver * const videoDriver_ { device_.get()? device_->getVideoDriver()    : nullptr };
        irr::scene::ISceneManager* const sceneMan_    { device_.get()? device_->getSceneManager()   : nullptr };
        irr::gui::IGUIEnvironment* const guiEnv_      { device_.get()? device_->getGUIEnvironment() : nullptr };
        irr::scene::ICameraSceneNode*    camera_      { nullptr }; 

        // Match the viewport to the physical surface of the Retina/HiDPI.
        void configureViewport();
    };

    } // namespace irrinterface
} // namespace game 
