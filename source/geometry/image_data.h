#pragma once

class image_data
{
public:

  class const_iterator
  {

  public:

    using difference_type = std::ptrdiff_t;
    using value_type = std::tuple<int, int, char>;

    const_iterator(const image_data* container, int column, int row);

    auto operator++() -> const_iterator&;
    auto operator++(int) -> const_iterator;
    auto operator*() const -> const value_type&;
    auto operator==(const const_iterator& i) const -> bool;

  private:

    const image_data* m_container;
    int m_column;
    int m_row;
    value_type m_value;

  };

  friend class const_iterator;

  image_data(std::ranges::input_range auto&& data);
  [[nodiscard]] auto begin() const -> const_iterator;
  [[nodiscard]] auto end() const -> const_iterator;

private:

  [[nodiscard]] auto Get(int m_column, int m_row) const -> char;
  [[nodiscard]] auto Next(int m_column, int m_row) const -> const_iterator;

  std::vector<std::string_view> m_data;

};

inline image_data::const_iterator::const_iterator(const image_data* container, int column, int row)
  : m_container { container }, m_column { column}, m_row { row }, m_value { column, row, m_container->Get(column, row) }
{
}

inline auto image_data::const_iterator::operator++() -> const_iterator&
{
  auto next = m_container->Next(m_column, m_row);
  m_column = next.m_column;
  m_row = next.m_row;
  m_value = { m_column, m_row, m_container->Get(m_column, m_row) };
  return *this;
}

inline auto image_data::const_iterator::operator++(int) -> const_iterator
{
  auto thisCopy = *this;
  auto next = m_container->Next(m_column, m_row);
  m_column = next.m_column;
  m_row = next.m_row;
  m_value = { m_column, m_row, m_container->Get(m_column, m_row) };
  return thisCopy;
}

inline auto image_data::const_iterator::operator*() const -> const value_type&
{
  return m_value;
}

inline auto image_data::const_iterator::operator==(const const_iterator& i) const -> bool
{
  return m_container == i.m_container && m_column == i.m_column && m_row == i.m_row;
}

image_data::image_data(std::ranges::input_range auto&& data)
{
  std::ranges::copy(data, std::back_inserter(m_data));
}

inline [[nodiscard]] auto image_data::begin() const -> const_iterator
{
  return const_iterator { this, 0, 0 };
}

inline [[nodiscard]] auto image_data::end() const -> const_iterator
{
  return const_iterator { this, 0, static_cast<int>(m_data.size()) };
}

inline [[nodiscard]] auto image_data::Get(int m_column, int m_row) const -> char
{
  return m_row < m_data.size() ? m_data[m_row][m_column] : ' ';
}

inline [[nodiscard]] auto image_data::Next(int column, int row) const -> const_iterator
{
  auto nextColumn = column + 1 < m_data[row].size() ? column + 1 : 0;
  auto nextRow = nextColumn == 0 ? row + 1 : row;
  return { this, nextColumn, nextRow };
}
