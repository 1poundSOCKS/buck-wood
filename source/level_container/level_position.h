#pragma once

#include "level_cell_collection.h"

class level_position
{

public:

  level_position(POINT_2F value);
  [[nodiscard]] auto Get() const -> POINT_2F;
  auto MoveBy(POINT_2F value, const level_cell_collection& cells, SIZE_F objectSize) -> POINT_2F;

private:

  static [[nodiscard]] auto ExpandRect(RECT_F rect, SIZE_F size) -> RECT_F;

private:

  POINT_2F m_value;

};

inline [[nodiscard]] auto level_position::Get() const -> POINT_2F
{
  return m_value;
}
