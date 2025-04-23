#pragma once
#include<iostream>
#include<engine/man/entitymanager.hpp>
#include<game/cmp/aicmp.hpp>
#include<game/cmp/physicscmp.hpp>
#include<game/cmp/rendercmp.hpp>

void seetype(auto) { std::cout<<__PRETTY_FUNCTION__<< '\n'; }

// Game aliases
namespace GAME {

// TAGS
struct TGPlayer_t {};
struct TGEnemy_t  {};
struct TGBullet_t {};
struct TGPursue_t {};

} // namespace GAME


// Engine aliases
namespace UVENGINE {

using CMP_PACK     = UVENGINE::Pack_t<GAME::RenderCmp_t, GAME::AICmp_t, GAME::PhysicsCmp_t>;
using TAG_PACK     = UVENGINE::Pack_t<GAME::TGPlayer_t, GAME::TGEnemy_t, GAME::TGBullet_t, GAME::TGPursue_t>;
using CmpStorage_t = UVENGINE::ComponentStorage_t<CMP_PACK, TAG_PACK>;
using EManager_t   = UVENGINE::EntityManager_t<CMP_PACK, TAG_PACK>; 
using Entity_t     = UVENGINE::EntityManager_t<CMP_PACK, TAG_PACK>::Entity_t; 

} // namespace UVENGINE
