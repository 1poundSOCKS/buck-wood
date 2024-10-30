#pragma once

#include "cell_id.h"

class cell_path
{

public:

  class const_iterator
  {

  public:

    enum class type { none, begin, end };

    using difference_type = std::ptrdiff_t;
    using value_type = cell_id;

    const_iterator(const cell_path& c, cell_id cellId);

    auto operator++() -> const_iterator&;
    auto operator++(int) -> const_iterator;
    auto operator*() const -> const cell_id&;
    auto operator==(const const_iterator& i) const -> bool;

  private:

    const cell_path& m_container;
    cell_id m_cellId;

  };

  friend class const_iterator;

  cell_path(cell_id begin, cell_id end);

  [[nodiscard]] auto begin() const -> const_iterator;
  [[nodiscard]] auto end() const -> const_iterator;

private:

  [[nodiscard]] auto Next(cell_id cellId) const noexcept -> cell_id;

  cell_id m_begin;
  cell_id m_end;

};
