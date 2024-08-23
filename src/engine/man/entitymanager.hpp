#pragma once
#include<vector>
#include "componentstorage.hpp"

namespace UVENGINE {

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
    //ComponentStorage_t<>
};

} // namespace UVENGINE