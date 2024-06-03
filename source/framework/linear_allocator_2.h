#pragma once

#include "linear_buffer.h"

class linear_allocator_state
{

public:

    linear_allocator_state() noexcept
    {
    }

    BYTE* allocate(const size_t size, const size_t n);
    void deallocate(BYTE* const p, size_t) noexcept;

    [[nodiscard]] auto AllocationCount() const -> size_t
    {
        return m_count - m_freeMemory.size();
    }

private:

    [[nodiscard]] auto get(size_t index) const noexcept -> BYTE*
    {
        return m_buffer.get() + index * m_size;
    }

    [[nodiscard]] auto contains(BYTE* object) const noexcept -> bool
    {
      return object >= m_buffer.get() && object < m_buffer.get() + m_count * m_size;
    }

    inline static constexpr size_t m_count { 1000 };
    size_t m_size { 0 };
    std::unique_ptr<BYTE[]> m_buffer;
    std::stack<BYTE*,std::vector<BYTE*>> m_freeMemory;
};

inline BYTE* linear_allocator_state::allocate(const size_t size, const size_t n)
{
    if( m_buffer == nullptr )
    {
        m_size = size;
        m_buffer = std::make_unique<BYTE[]>(m_size * m_count);

        for( size_t i = m_count; i > 0; --i )
        {
            m_freeMemory.push(get(i-1));
        }
    }

    if( size != m_size )
    {
        return new BYTE[size * n];
    }

    auto allocated = m_freeMemory.top();
    m_freeMemory.pop();
    return allocated;
}

inline void linear_allocator_state::deallocate(BYTE * const p, size_t) noexcept
{
    if( contains(p) )
    {
        m_freeMemory.push(p);
    }
    else
    {
        delete [] reinterpret_cast<BYTE*>(p);
    }
}

template <typename T> class linear_allocator_2
{

public:

    typedef T value_type;

    linear_allocator_2(linear_allocator_state& state) noexcept : m_state { state }
    {
        // for( size_t i = S; i > 0; --i )
        // {
        //     m_freeMemory.push(m_buffer.get(i-1));
        // }
    }

    template<class U> linear_allocator_2(const linear_allocator_2<U>& allocator) noexcept : m_state { allocator.m_state }
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

    T* allocate(const size_t n);
    void deallocate(T* const p, size_t) noexcept;

    template<typename U> struct rebind
    {
        typedef linear_allocator_2<U> other;
    };

    linear_allocator_state& m_state;

private:

    // linear_buffer<T, S> m_buffer;
    // std::stack<T*,std::vector<T*>> m_freeMemory;

};

template <class T> T* linear_allocator_2<T>::allocate(const size_t n)
{
    return reinterpret_cast<T*>(m_state.allocate(sizeof(T), n));
}

template<class T> void linear_allocator_2<T>::deallocate(T * const p, size_t n) noexcept
{
    return m_state.deallocate(reinterpret_cast<BYTE*>(p), n * sizeof(T));
}
