#pragma once

class row_def
{

public:

  using column_collection = std::vector<D2D1_RECT_F>;

  row_def(const D2D1_RECT_F& area, size_t columns);

  [[nodiscard]] auto begin() const -> column_collection::const_iterator;
  [[nodiscard]] auto end() const -> column_collection::const_iterator;

private:

  column_collection m_columns;

};

[[nodiscard]] inline auto row_def::begin() const -> column_collection::const_iterator
{
  return std::begin(m_columns);
}

[[nodiscard]] inline auto row_def::end() const -> column_collection::const_iterator
{
  return std::end(m_columns);
}
