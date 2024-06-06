#pragma once

class level_cell_item
{

public:

  level_cell_item(int x, int y, level_cell_type cellType, POINT_2I position) : 
    level_cell_item { x, y, cellType, POINT_2F { static_cast<float>(position.x), static_cast<float>(position.y) } }
  {
  }

  level_cell_item(int x, int y, level_cell_type cellType, POINT_2F position) :
    m_x { x }, m_y { y }, m_cellType { cellType }, m_position { position }
  {
  }

  [[nodiscard]] auto Type() const noexcept -> level_cell_type
  {
    return m_cellType;
  }

  [[nodiscard]] auto X() const noexcept -> int
  {
    return m_x;
  }

  [[nodiscard]] auto Y() const noexcept -> int
  {
    return m_y;
  }

  [[nodiscard]] auto Position() const noexcept -> POINT_2F
  {
    return m_position;
  }

private:

  level_cell_type m_cellType;
  int m_x;
  int m_y;
  POINT_2F m_position;

};
