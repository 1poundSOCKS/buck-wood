#pragma once

#include "framework.h"
#include "base_object.h"

class portal : public base_object
{

public:

  portal(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity);

  auto SetCellId(POINT_2I cellId) -> void;

  [[nodiscard]] auto CellId() const -> std::optional<POINT_2I>;

private:

  std::optional<POINT_2I> m_cellId;

};

inline portal::portal(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : base_object { position, scale, angle }
{
}

inline auto portal::SetCellId(POINT_2I cellId) -> void
{
  m_cellId = cellId;
}

inline auto portal::CellId() const -> std::optional<POINT_2I>
{
  return m_cellId;
}
