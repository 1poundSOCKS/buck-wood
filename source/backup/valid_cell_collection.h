#pragma once

#include "valid_cell.h"

class valid_cell_collection
{

public:

  using value_type = std::vector<valid_cell>;
  using back_inserter_type = std::back_insert_iterator<value_type>;

  auto Load(ID2D1Geometry* boundary, float cellWidth, float cellHeight) -> void;

  [[nodiscard]] auto Get() const noexcept -> const value_type&
  {
    return m_value;
  }

  [[nodiscard]] auto BackInserter() noexcept -> back_inserter_type
  {
    return std::back_inserter(m_value);
  }

  auto EmplaceBack(auto...args) noexcept -> void
  {
    m_value.emplace_back(std::forward<decltype(args)>(args)...);
  }

  [[nodiscard]] auto CellPosition(int x, int y) const noexcept-> POINT_2F;

private:

  [[nodiscard]] auto CellTopLeft() const noexcept -> POINT_2F;
  [[nodiscard]] auto CellBottomRight() const noexcept -> POINT_2F;
  [[nodiscard]] auto CellRect() const noexcept -> RECT_F;

  [[nodiscard]] auto MinCellX() const noexcept -> int;
  [[nodiscard]] auto MinCellY() const noexcept -> int;
  [[nodiscard]] auto MaxCellX() const noexcept -> int;
  [[nodiscard]] auto MaxCellY() const noexcept -> int;

  [[nodiscard]] auto CellGeometry(int x, int y) const noexcept -> winrt::com_ptr<ID2D1TransformedGeometry>;
  [[nodiscard]] auto CellIsValid(ID2D1Geometry *boundary, ID2D1Geometry* cell) const noexcept -> bool;

private:

  float m_cellWidth { 0 };
  float m_cellHeight { 0 };
  RECT_F m_boundaryRect;
  winrt::com_ptr<ID2D1PathGeometry> m_cellGeometry;
  value_type m_value;

};
