#pragma once 
#include<cstdint>
#include<array>
#include<stdexcept>

//#define NDEBUG
#include<cassert>

namespace ENGINE {

template<typename DATA_TYPE, std::size_t CAPACITY=10, typename INDEX_TYPE=std::size_t>
struct Slotmap_t {

    using data_type = DATA_TYPE;
    using uint_type = INDEX_TYPE;
    using key_type  = struct { uint_type id_; uint_type gen_; };

    explicit constexpr Slotmap_t() noexcept
    {
        freeKeys_init();
    }

    [[nodiscard]] constexpr uint_type size()     const noexcept { return size_;    } // Attibute 'nodiscard': means that the return of method can't be ignored.
    [[nodiscard]] constexpr uint_type capacity() const noexcept { return CAPACITY; }

// ITERATORS TO DATA
    [[nodiscard]] constexpr typename std::array<DATA_TYPE, CAPACITY>::iterator begin()              noexcept { return data_.begin();          }
    [[nodiscard]] constexpr typename std::array<DATA_TYPE, CAPACITY>::iterator end()                noexcept { return data_.begin() + size_;  }
    [[nodiscard]] constexpr typename std::array<DATA_TYPE, CAPACITY>::const_iterator cbegin() const noexcept { return data_.cbegin();         }
    [[nodiscard]] constexpr typename std::array<DATA_TYPE, CAPACITY>::const_iterator cend()   const noexcept { return data_.cbegin() + size_; }

// INSERT
    [[nodiscard]] constexpr key_type insert(data_type&& element) // R-values only!, no universal reference (the type is deducted already).
    {
        uint_type key_pos { occupy_freeKey() };

        // Insert element
        data_ [size_- 1] = std::move(element); // Move resource
        erase_[size_- 1] = key_pos;

        // Make key for user and return
        return key_type { key_pos, generation_ };
    }

// ERASE
    constexpr bool erase(key_type const& key_user) noexcept
    {
        bool erase_okey {false};

        if (is_valid(key_user)) 
        {
            uint_type pos_data { liberate_key(key_user) };
            erase_element(pos_data);

            --size_;            // Update size
            erase_okey = true;  // Successful deletion
        }
        return erase_okey;
    }

// ACCESS TO ELEMENT WITH KEY
    constexpr data_type const& operator[](key_type const& key_user) const noexcept
    {
        assert( is_valid(key_user) && "Invalid key user" ); // Verify only debugging

        // Accessing the key object and get id_ (position if data to get)
        uint_type pos_data { indices_[key_user.id_].id_ };

        return data_[pos_data];
    }

    constexpr data_type& operator[](key_type const& key_user) noexcept
    {
        data_type const& element { const_cast<Slotmap_t const*>(this)->operator[](key_user) };
        return *const_cast<data_type*>(&element);
    }

private:

    constexpr void erase_element(uint_type pos_data) noexcept
    {
        // Get position of end element
        uint_type end_pos {size_ - 1};

        // If the element is in the mid, replace it with the end element
        if (pos_data < end_pos) 
        {
            // Copy end element to current
            data_[pos_data]  = data_[end_pos];
            erase_[pos_data] = erase_[end_pos];

            // Then, you have to update its corresponding element in the array indices_
            indices_[erase_[pos_data]].id_ = pos_data;

        }  // The element is the end, nothing to do.
    }

    [[nodiscard]] constexpr uint_type liberate_key(key_type const& key_user) noexcept
    {
        assert( is_valid(key_user) ); // Verify only debugging

        // Accessing the key object to free
        key_type& key_to_free { indices_[key_user.id_] };

        // Before to free, store id_ (corresponds to the position where the element to be removed is, and value to return)
        uint_type pos_data { key_to_free.id_ };

        // Free key and update freelist
        key_to_free.id_  = freelist_;
        key_to_free.gen_ = MAX_VALUE_;    // Assuming that the generation never reaches the maximum value!
        freelist_        = key_user.id_;

        return pos_data;
    }

    constexpr bool is_valid(key_type const& key_user) const noexcept
    {
        return (key_user.id_ < CAPACITY && indices_[key_user.id_].gen_ == key_user.gen_);
    }

    [[nodiscard]] constexpr uint_type occupy_freeKey()
    {   
        if (size_ >= CAPACITY) throw std::runtime_error("Insufficient capacity in slotmap"); // Run time error
        assert(freelist_ < CAPACITY); // Only verify with debug

        uint_type free_pos { freelist_ };           // Save the unoccupied key position
        key_type& free_key { indices_[free_pos] };  // Get unoccupied key
        freelist_ = free_key.id_;                   // Update freelist
    
        // Occupy key and update generation and size
        free_key.id_  = size_++;          // size_ is the position of the new element in data[]!
        free_key.gen_ = ++generation_;    // Tracing

        return free_pos;
    }

    constexpr void freeKeys_init() noexcept
    {
        // Set next_free for each key
        uint_type next_free {1};
        for (auto& key : indices_) 
            key.id_ = next_free++;
        freelist_ = 0;
    }

    char const b_indice[16] = "INDICES########";
    std::array<key_type,     CAPACITY> indices_{};

    char const b_data[16]   = "DATA###########";
    std::array<data_type, CAPACITY> data_{};      // Cmps!

    char const b_erase[16]   = "ERASE##########";
    std::array<uint_type,    CAPACITY> erase_{};

    char const b_free[8]   = "FREE###";
    uint_type freelist_{0};

    char const b_gen[8]    = "GEN####";
    uint_type generation_{0};

    char const b_size[8]   = "SIZE###";
    uint_type size_{0};

    char const b_capacity[8] = "CAPA###";
    uint_type capa_{CAPACITY};

    static constexpr uint_type MAX_VALUE_ { std::numeric_limits<uint_type>::max() };
};

} // namespace ENGINE