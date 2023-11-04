#pragma once

class column_def
{

public:

  using row_collection = std::vector<D2D1_RECT_F>;

  column_def(const D2D1_RECT_F& area, size_t rows);

  [[nodiscard]] auto begin() const -> row_collection::const_iterator;
  [[nodiscard]]auto end() const -> row_collection::const_iterator;

private:

  row_collection m_rows;

};

[[nodiscard]] inline auto column_def::begin() const -> row_collection::const_iterator
{
  return std::begin(m_rows);
}

[[nodiscard]] inline auto column_def::end() const -> row_collection::const_iterator
{
  return std::end(m_rows);
}
