#pragma once
#include<engine/man/entitymanager.hpp>
#include<game/cmp/aicmp.hpp>
#include<game/cmp/physicscmp.hpp>
#include<game/cmp/rendercmp.hpp>

// Game aliases
namespace game {

// TAGS
struct TGPlayer {};
struct TGEnemy  {};
struct TGBullet {};
struct TGPursue {};
struct TGSuperKick {};
struct TGSuperJump {};

} // namespace GAME


// Engine aliases
namespace uvengcfg {

using CMP_PACK     = uvengine::TypePack_t<game::RenderCmp_t, game::AICmp_t, game::PhysicsCmp_t>;
using TAG_PACK     = uvengine::TypePack_t<game::TGPlayer, game::TGEnemy, game::TGBullet, game::TGPursue, game::TGSuperKick, game::TGSuperJump>;
using EManager_t   = uvengine::EntityManager_t<CMP_PACK, TAG_PACK>;
using CmpStorage_t = EManager_t::CmpStorage_t;
using Entity_t     = EManager_t::Entity_t;

} // namespace uvengine
