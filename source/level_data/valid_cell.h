#pragma once

class valid_cell
{

// public:

  // enum class cell_type { wall, floor };

public:

  valid_cell(int x, int y, level_cell_type cellType, POINT_2F position, winrt::com_ptr<ID2D1TransformedGeometry> geometry) :
    m_x { x }, m_y { y }, m_cellType { cellType }, m_position { position }, m_geometry { geometry }
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

  [[nodiscard]] auto Geometry() const noexcept -> winrt::com_ptr<ID2D1TransformedGeometry>
  {
    return m_geometry;
  }

private:

  level_cell_type m_cellType;
  int m_x;
  int m_y;
  POINT_2F m_position;
  winrt::com_ptr<ID2D1TransformedGeometry> m_geometry;

};
