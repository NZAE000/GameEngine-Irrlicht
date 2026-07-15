#pragma once
#include<engine/util/vector3.hpp>


namespace game {

struct PhysicsCmp_t {

    using Vec3_t = uvengine::math::Vect3_t<float>;

    Vec3_t position_{};
    Vec3_t velocity_{};
    //float _x{}, _y{}, _z{};
    //float _vx{}, _vy{}, _vz{};
};

} // namespace GAME
