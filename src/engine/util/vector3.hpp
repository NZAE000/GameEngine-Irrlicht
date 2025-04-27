#pragma once
#include<cmath>

namespace uvengine { 


template<typename TYPE>
struct Vect3_t {

    constexpr Vect3_t() = default;
    constexpr Vect3_t(TYPE x, TYPE y, TYPE z) 
    : x_{x}, y_{y}, z_{z} {}


// OPERATORS ! #####################################################################################

// Spaceship operator (==, !=, >, <, >=, <=)
    constexpr auto operator<=>(Vect3_t const& rhs_op) const noexcept = default;

// Sum
    constexpr Vect3_t operator+(Vect3_t const& rhs_op) const noexcept { // rhs = right hand side.
        return {
            x_ + rhs_op.x_,
            y_ + rhs_op.y_,
            z_ + rhs_op.z_
        };
    }

// Sub
    constexpr Vect3_t operator-(Vect3_t const& rhs_op) const noexcept {
        return {
            x_ - rhs_op.x_,
            y_ - rhs_op.y_,
            z_ - rhs_op.z_
        };
    }

// Dot product operators
    constexpr TYPE operator*(Vect3_t const& rhs_op) const noexcept {
        return x_ * rhs_op.x_ + 
               y_ * rhs_op.y_ + 
               z_ * rhs_op.z_;
    }
    constexpr Vect3_t operator*(TYPE const scale) const noexcept { // this * n
        return { 
            scale * x_, 
            scale * y_ , 
            scale * z_  
        };
    }
    friend constexpr Vect3_t operator*(TYPE const scale, Vect3_t const& vec) noexcept { // Available n * this
        return vec * scale;
    }

// Distance
    constexpr TYPE lengthSqrt(void) const noexcept { return x_*x_ + y_*y_ + z_*z_;  } // Distane without square root
    constexpr TYPE length(void)     const noexcept { return std::sqrt(lengthSqrt()); }

// Normalize
    constexpr Vect3_t& normalize(void) noexcept
    {
        x_ /= length();
        y_ /= length();
        z_ /= length();
        return *this;
    }


private:
    TYPE x_{}, y_{}, z_{}; // zero default

};

} // namespace uvengine 
