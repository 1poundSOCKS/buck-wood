#pragma once

#include "framework.h"
#include "base_object.h"
#include "cell_id.h"

class portal : public base_object
{

public:

  portal(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity);

  auto SetCellId(cell_id cellId) -> void;

  [[nodiscard]] auto CellId() const -> std::optional<cell_id>;

private:

  std::optional<cell_id> m_cellId;

};

inline portal::portal(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : base_object { position, scale, angle }
{
}

inline auto portal::SetCellId(cell_id cellId) -> void
{
  m_cellId = cellId;
}

inline auto portal::CellId() const -> std::optional<cell_id>
{
  return m_cellId;
}
