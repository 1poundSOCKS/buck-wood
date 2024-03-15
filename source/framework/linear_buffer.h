#pragma once

template <typename T, size_t S> class linear_buffer
{

public:

    linear_buffer() : m_buffer { reinterpret_cast<T*>(new BYTE[sizeof(T) * S]) }
    {
    }

    ~linear_buffer()
    {
        auto buffer = reinterpret_cast<BYTE*>(m_buffer.release());
        delete [] buffer;
    }

    [[nodiscard]] auto get(size_t index) const noexcept -> T*
    {
        return m_buffer.get() + index;
    }

    [[nodiscard]] auto contains(T* object) const noexcept -> bool
    {
      return object >= m_buffer.get() && object < m_buffer.get() + S;
    }

private:

    std::unique_ptr<T[]> m_buffer;
};
