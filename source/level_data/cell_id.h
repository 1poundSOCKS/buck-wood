#pragma once

#include "framework.h"

class cell_id
{

public:

  cell_id(int column, int row) : m_column { column }, m_row { row }
  {
  }

  // [[nodiscard]] auto Column() const noexcept -> int
  // {
  //   return m_column;
  // }

  // [[nodiscard]] auto Row() const noexcept -> int
  // {
  //   return m_row;
  // }

  [[nodiscard]] auto Position() const noexcept -> POINT_2I
  {
    return { m_column, m_row };
  }

  // [[nodiscard]] auto ToTuple() const noexcept -> std::tuple<int, int>
  // {
  //   return { m_column, m_row };
  // }

private:

  int m_column;
  int m_row;

};
