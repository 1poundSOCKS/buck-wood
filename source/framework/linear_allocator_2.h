#pragma once

#include "linear_allocator_state.h"

template <typename T> class linear_allocator_2
{

public:

    typedef T value_type;

    linear_allocator_2(linear_allocator_state& state) noexcept : m_state { state }
    {
    }

    template<class U> linear_allocator_2(const linear_allocator_2<U>& allocator) noexcept : m_state { allocator.state() }
    {
    }

    template<class U> bool operator==(const linear_allocator_2<U>&) const noexcept
    {
        return true;
    }

    template<class U> bool operator!=(const linear_allocator_2<U>&) const noexcept
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
        typedef linear_allocator_2<U> other;
    };

    [[nodiscard]] auto state() const -> linear_allocator_state&
    {
        return m_state;
    }

private:

    linear_allocator_state& m_state;

};
