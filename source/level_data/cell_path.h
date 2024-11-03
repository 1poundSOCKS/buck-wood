#pragma once

#include "framework.h"

class cell_path
{

public:

  class const_iterator
  {

  public:

    enum class type { none, begin, end };

    using difference_type = std::ptrdiff_t;
    using value_type = POINT_2I;

    const_iterator() = default;
    const_iterator(const cell_path* c, POINT_2I cellId);

    auto operator++() -> const_iterator&;
    auto operator++(int) -> const_iterator;
    auto operator*() const -> const POINT_2I&;
    auto operator==(const const_iterator& i) const -> bool;

  private:

    const cell_path* m_container { nullptr };
    POINT_2I m_cellId;

  };

  friend class const_iterator;

  cell_path(POINT_2I begin, POINT_2I end);

  [[nodiscard]] auto begin() const -> const_iterator;
  [[nodiscard]] auto end() const -> const_iterator;

private:

  [[nodiscard]] auto Next(POINT_2I cellId) const noexcept -> POINT_2I;

  POINT_2I m_begin;
  POINT_2I m_end;

};
