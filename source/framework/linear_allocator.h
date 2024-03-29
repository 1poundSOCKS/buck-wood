#pragma once

#include "linear_buffer.h"

template <typename T, size_t S> class linear_allocator
{

public:

    typedef T value_type;

    linear_allocator() noexcept
    {
        for( size_t i = S; i > 0; --i )
        {
            m_freeMemory.push(m_buffer.get(i-1));
        }
    }

    template<class U> linear_allocator(const linear_allocator<U, S>&) noexcept {}

    template<class U> bool operator==(const linear_allocator<U, S>&) const noexcept
    {
        return true;
    }

    template<class U> bool operator!=(const linear_allocator<U, S>&) const noexcept
    {
        return false;
    }

    T* allocate(const size_t n);
    void deallocate(T* const p, size_t) noexcept;

    template<typename U> struct rebind
    {
        typedef linear_allocator<U, S> other;
    };

private:

    linear_buffer<T, S> m_buffer;
    std::stack<T*,std::vector<T*>> m_freeMemory;

};

template <class T, size_t S> T* linear_allocator<T, S>::allocate(const size_t n)
{
    if( m_freeMemory.size() == 0 || n > 1 )
    {
        return reinterpret_cast<T*>(new BYTE[n * sizeof T]);
    }
    
    if( n > static_cast<size_t>(-1) / sizeof(T) )
    {
        throw std::bad_array_new_length();
    }
    
    auto allocated = m_freeMemory.top();
    m_freeMemory.pop();
    return allocated;
}

template<class T, size_t SIZE> void linear_allocator<T, SIZE>::deallocate(T * const p, size_t n) noexcept
{
    if( m_buffer.contains(p) )
    {
        m_freeMemory.push(p);
    }
    else
    {
        delete [] reinterpret_cast<BYTE*>(p);
    }
}
