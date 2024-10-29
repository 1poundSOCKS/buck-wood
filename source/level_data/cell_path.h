#pragma once

#include "cell_id.h"

class cell_path;

class cell_path_iterator
{

public:

  enum class type { none, begin, end };

  using difference_type = std::ptrdiff_t;
  using value_type = cell_id;

  cell_path_iterator(const cell_path& cellPath, cell_id cellId);

  auto operator++() -> cell_path_iterator&;
  auto operator++(int) -> cell_path_iterator;
  auto operator*() const -> const cell_id&;
  auto operator==(const cell_path_iterator& i) const -> bool;

private:

  const cell_path& m_cellPath;
  cell_id m_cellId;

};

class cell_path
{

public:

  friend class cell_path_iterator;

  cell_path(cell_id begin, cell_id end);

  [[nodiscard]] auto begin() const -> cell_path_iterator;
  [[nodiscard]] auto end() const -> cell_path_iterator;

private:

  [[nodiscard]] auto Next(cell_id cellId) const noexcept -> cell_id;

  cell_id m_begin;
  cell_id m_end;

};
