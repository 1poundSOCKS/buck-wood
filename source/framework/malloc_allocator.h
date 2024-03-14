#pragma once

template <class T>
struct malloc_allocator
{
    typedef T value_type;
    malloc_allocator() noexcept {}

    template<class U> malloc_allocator(const malloc_allocator<U>&) noexcept {}

    template<class U> bool operator==(const malloc_allocator<U>&) const noexcept
    {
        return true;
    }

    template<class U> bool operator!=(const malloc_allocator<U>&) const noexcept
    {
        return false;
    }

    T* allocate(const size_t n) const;
    void deallocate(T* const p, size_t) const noexcept;

    template<typename U>
    struct rebind {
        typedef malloc_allocator<U> other;
    };
};

template <class T>
T* malloc_allocator<T>::allocate(const size_t n) const
{
    if (n == 0)
        return nullptr;
    
    if (n > static_cast<size_t>(-1) / sizeof(T))
    {
        throw std::bad_array_new_length();
    }
    
    void* const pv = malloc(n * sizeof(T));
    
    if( !pv )
        throw std::bad_alloc();
    
    return static_cast<T*>(pv);
}

template<class T>
void malloc_allocator<T>::deallocate(T * const p, size_t) const noexcept
{
    free(p);
}
