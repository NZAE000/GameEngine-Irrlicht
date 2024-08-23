#pragma once
#include "slotmap.hpp"
#include<tuple>

namespace UVENGINE {

using CmpType_t = std::uint8_t;

template<typename... CMPS>
using Tuple_t = std::tuple<CMPS...>;


template<typename CMP1, typename CMP2, typename CMP3, std::size_t CAPACITY=100>
struct ComponentStorage_t {

    explicit ComponentStorage_t() = default;

    template<typename CMP_t>
    [[nodiscard]] constexpr CmpType_t getCmpId()  const noexcept { return getCmpId(CMP_t{}); }

    template<typename CMP_t>
    [[nodiscard]] constexpr std::size_t getMask() const noexcept { return 1 << getCmpId<CMP_t>(); }

    template<typename CMP_t> // Get slotmap of CMP_t
    [[nodiscard]] constexpr Slotmap_t<CMP_t, CAPACITY>& getContainer()
    {   
        return std::get<Slotmap_t<CMP_t, CAPACITY>>(components_);
    }

private:
// Container of different components
    Tuple_t<Slotmap_t<CMP1, CAPACITY>, Slotmap_t<CMP2, CAPACITY>, Slotmap_t<CMP3, CAPACITY>> components_{};

    [[nodiscard]] constexpr CmpType_t getCmpId(CMP1) const noexcept { return 0; }
    [[nodiscard]] constexpr CmpType_t getCmpId(CMP2) const noexcept { return 1; }
    [[nodiscard]] constexpr CmpType_t getCmpId(CMP3) const noexcept { return 2; }
};

} // namespace UVENGINE