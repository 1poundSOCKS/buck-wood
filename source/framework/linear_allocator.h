#pragma once

template <typename T> struct linear_allocator
{
    typedef T value_type;

    linear_allocator() noexcept : m_buffer { reinterpret_cast<T*>(new BYTE[sizeof(T) * 10000]) }
    {
        for( int i = 10000 - 1; i >= 0; --i )
        {
            m_freeMemory.push(m_buffer.get() + i);
        }
    }

    ~linear_allocator()
    {
        auto buffer = reinterpret_cast<BYTE*>(m_buffer.release());
        delete [] buffer;
    }

    template<class U> linear_allocator(const linear_allocator<U>&) noexcept {}

    template<class U> bool operator==(const linear_allocator<U>&) const noexcept
    {
        return true;
    }

    template<class U> bool operator!=(const linear_allocator<U>&) const noexcept
    {
        return false;
    }

    T* allocate(const size_t n);
    void deallocate(T* const p, size_t) noexcept;

    template<typename U>
    struct rebind {
        typedef linear_allocator<U> other;
    };

    std::unique_ptr<T[]> m_buffer;
    std::stack<T*,std::vector<T*>> m_freeMemory;
};

template <class T>
T* linear_allocator<T>::allocate(const size_t n)
{
    if( n != 1 )
    {
        return nullptr;
    }
    
    if( n > static_cast<size_t>(-1) / sizeof(T) )
    {
        throw std::bad_array_new_length();
    }
    
    auto allocated = m_freeMemory.top();
    m_freeMemory.pop();
    return allocated;
}

template<class T>
void linear_allocator<T>::deallocate(T * const p, size_t) noexcept
{
    m_freeMemory.push(p);
}
