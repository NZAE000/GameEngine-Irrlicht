#include "irrinterface.hpp"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#endif
//#include<iostream>

namespace game {
    namespace irrinterface {

    GFXDevice_t::GFXDevice_t(u32 w, u32 h)
    : width_{w}, height_{h}
    {   
        irr::IrrlichtDevice* device { device_.get() };
        if (!device) throw std::runtime_error("Can't initialize irrlicht device"); // Check!!

        device->setWindowCaption(L"window - Irrlicht demo");    // Title
        camera_ = sceneMan_->addCameraSceneNodeFPS();           // Add camera (first person)!

        device_->getFileSystem()->changeWorkingDirectoryTo("GameEngine-Irrlicht");
        configureViewport();
    }

    // Match the viewport to the physical surface of the Retina/HiDPI.
    void GFXDevice_t::configureViewport()
    {
        #ifdef __APPLE__
            irr::core::dimension2du const& size { videoDriver_->getScreenSize() };
            constexpr GLsizei scale = 2;
            glViewport(
            0,
            0,
            static_cast<GLsizei>(size.Width) * scale,
            static_cast<GLsizei>(size.Height) * scale
        );
        #endif
    }

    irr::scene::ISceneNode& GFXDevice_t::
    createSphere(std::string_view texture_path)
    {
        irr::scene::ISceneNode* node = sceneMan_->addSphereSceneNode();
        if (!node) throw std::runtime_error("Couldn't create sphere");

        auto* texture { videoDriver_->getTexture(texture_path.data()) };
        if (!texture) throw std::runtime_error("Couldn't create texture");

        node->setPosition(irr::core::vector3df(0, 0, 30));
        node->setMaterialTexture(0, texture);
        node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        
        return *node;
    }


    irr::scene::ITerrainSceneNode& GFXDevice_t::
    createTerrain(std::string_view map_path, std::string_view texture_path)
    {
        // add terrain scene node
        irr::scene::ITerrainSceneNode* terrain = sceneMan_->addTerrainSceneNode(
            map_path.data(),
            0,                                          // parent node
            -1,                                         // node id
            irr::core::vector3df(0.0f, 0.0f, 0.0f),     // position
            irr::core::vector3df(0.0f, 0.0f, 0.0f),     // rotation
            irr::core::vector3df(1.0f, 1.0f, 1.0f),     // scale
            irr::video::SColor(255, 255, 255, 255),     // vertexColor
            5,                                          // maxLOD
            irr::scene::ETPS_17,                        // patchSize
            4                                           // smoothFactor
            );
        if (!terrain) throw std::runtime_error("Couldn't create terrain");

        terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        terrain->setMaterialTexture(0, videoDriver_->getTexture(texture_path.data())); // Set texture.
        terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);
        
        irr::core::aabbox3d<irr::f32> box = terrain->getBoundingBox();
        //std::cout<<"max depth x_ "<< box.MaxEdge.X <<"\n";
        //std::cout<<"max depth z_ "<< box.MaxEdge.Z <<"\n";
        irr::f32 terrainWidth = box.MaxEdge.X - box.MinEdge.X;  // heightmap size in pixels (X).
        irr::f32 terrainDepth = box.MaxEdge.Z - box.MinEdge.Z;  // size in Z.
        irr::f32 newX         = -terrainWidth / 2.0f;
        irr::f32 newY         = -box.MinEdge.Y;
        irr::f32 newZ         = -terrainDepth / 2.0f;
        
        terrain->setPosition({newX, newY, newZ}); // Center terrain to 0,0,0.
        auto newbox = terrain->getBoundingBox();
        //std::cout << "Altura mínima del terreno: " << newbox.MinEdge.Y << std::endl;
        //std::cout << "Altura máxima del terreno: " << newbox.MaxEdge.Y << std::endl;
        //std::cout << "Profundidad máxima del terreno: " << newbox.MaxEdge.Z << std::endl;
        terrain->scaleTexture(1.0f, 1.0f);

        return *terrain;
    }

    } // namespace irrinterface
} // namespace game