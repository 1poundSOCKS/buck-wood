#pragma once

#include "custom_allocator_state.h"

template <typename T> class custom_allocator
{

public:

    typedef T value_type;

    custom_allocator(custom_allocator_state& state) noexcept : m_state { state }
    {
    }

    template<class U> custom_allocator(const custom_allocator<U>& allocator) noexcept : m_state { allocator.state() }
    {
    }

    template<class U> bool operator==(const custom_allocator<U>&) const noexcept
    {
        return true;
    }

    template<class U> bool operator!=(const custom_allocator<U>&) const noexcept
    {
        return false;
    }

    T* allocate(const size_t n)
    {
        return reinterpret_cast<T*>(m_state.allocate(sizeof(T), n));
    }

    void deallocate(T * const p, size_t n) noexcept
    {
        return m_state.deallocate(reinterpret_cast<BYTE*>(p), n * sizeof(T));
    }

    template<typename U> struct rebind
    {
        typedef custom_allocator<U> other;
    };

    [[nodiscard]] auto state() const -> custom_allocator_state&
    {
        return m_state;
    }

private:

    custom_allocator_state& m_state;

};
