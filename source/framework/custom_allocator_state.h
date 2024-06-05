#pragma once

class custom_allocator_state
{

public:

    custom_allocator_state(size_t size, size_t count) noexcept : m_size { size }, m_count { count }
    {
        m_buffer = std::make_unique<BYTE[]>(m_size * m_count);

        for( size_t i = m_count; i > 0; --i )
        {
            m_freeMemory.push(get(i-1));
        }
    }

    BYTE* allocate(const size_t size, const size_t n);
    void deallocate(BYTE* const p, size_t) noexcept;

    [[nodiscard]] auto AllocationCount() const -> size_t
    {
        return m_count - m_freeMemory.size();
    }

    [[nodiscard]] auto HeapAllocationCount() const -> size_t
    {
        return m_heapAllocations;
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

    size_t m_size;
    size_t m_count;
    std::unique_ptr<BYTE[]> m_buffer;
    std::stack<BYTE*,std::vector<BYTE*>> m_freeMemory;
    size_t m_heapAllocations { 0 };
};

inline BYTE* custom_allocator_state::allocate(const size_t size, const size_t n)
{
    auto requiredBufferSize = size * n;

    if( requiredBufferSize > m_size || m_freeMemory.size() == 0 )
    {
        ++m_heapAllocations;
        return new BYTE[requiredBufferSize];
    }
    else
    {
        auto allocated = m_freeMemory.top();
        m_freeMemory.pop();
        return allocated;
    }
}

inline void custom_allocator_state::deallocate(BYTE * const p, size_t) noexcept
{
    if( contains(p) )
    {
        m_freeMemory.push(p);
    }
    else
    {
        --m_heapAllocations;
        delete [] reinterpret_cast<BYTE*>(p);
    }
}
