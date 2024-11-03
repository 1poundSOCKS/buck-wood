#pragma once

template <typename container_type>
class image_data
{
public:

  class const_iterator
  {

  public:

    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = std::tuple<int, int, char>;

    const_iterator() = default;
    const_iterator(const image_data* container, int column, int row);

    auto operator++() -> const_iterator&;
    auto operator++(int) -> const_iterator;
    auto operator*() const -> const value_type&;
    auto operator==(const const_iterator& i) const -> bool;

  private:

    const image_data* m_container { nullptr };
    int m_column { 0 };
    int m_row { 0 };
    value_type m_value { 0, 0, ' ' };

  };

  friend class const_iterator;

  image_data(const container_type& data);
  
  [[nodiscard]] auto begin() const -> const_iterator;
  [[nodiscard]] auto end() const -> const_iterator;

private:

  [[nodiscard]] auto Get(int m_column, int m_row) const -> char;
  [[nodiscard]] auto Next(int m_column, int m_row) const -> const_iterator;

  const container_type& m_data;

};

template <typename container_type>
inline image_data<container_type>::const_iterator::const_iterator(const image_data* container, int column, int row)
  : m_container { container }, m_column { column}, m_row { row }, m_value { column, row, m_container->Get(column, row) }
{
}

template <typename container_type>
inline auto image_data<container_type>::const_iterator::operator++() -> const_iterator&
{
  auto next = m_container->Next(m_column, m_row);
  m_column = next.m_column;
  m_row = next.m_row;
  m_value = { m_column, m_row, m_container->Get(m_column, m_row) };
  return *this;
}

template <typename container_type>
inline auto image_data<container_type>::const_iterator::operator++(int) -> const_iterator
{
  auto thisCopy = *this;
  auto next = m_container->Next(m_column, m_row);
  m_column = next.m_column;
  m_row = next.m_row;
  m_value = { m_column, m_row, m_container->Get(m_column, m_row) };
  return thisCopy;
}

template <typename container_type>
inline auto image_data<container_type>::const_iterator::operator*() const -> const value_type&
{
  return m_value;
}

template <typename container_type>
inline auto image_data<container_type>::const_iterator::operator==(const const_iterator& i) const -> bool
{
  return m_container == i.m_container && m_column == i.m_column && m_row == i.m_row;
}

template <typename container_type>
image_data<container_type>::image_data(const container_type& data) : m_data { data }
{
}

template <typename container_type>
inline [[nodiscard]] auto image_data<container_type>::begin() const -> const_iterator
{
  return const_iterator { this, 0, 0 };
}

template <typename container_type>
inline [[nodiscard]] auto image_data<container_type>::end() const -> const_iterator
{
  return const_iterator { this, 0, static_cast<int>(m_data.size()) };
}

template <typename container_type>
inline [[nodiscard]] auto image_data<container_type>::Get(int m_column, int m_row) const -> char
{
  return m_row < m_data.size() ? m_data[m_row][m_column] : ' ';
}

template <typename container_type>
inline [[nodiscard]] auto image_data<container_type>::Next(int column, int row) const -> const_iterator
{
  auto nextColumn = column + 1 < m_data[row].size() ? column + 1 : 0;
  auto nextRow = nextColumn == 0 ? row + 1 : row;
  return { this, nextColumn, nextRow };
}
