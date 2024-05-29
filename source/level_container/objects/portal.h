#pragma once

#include "framework.h"
#include "base_object.h"

class portal : public base_object
{

public:

  // enum class type { exit, entry };

  portal(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity);

  auto SetCellId(POINT_2I cellId) -> void;

  // [[nodiscard]] auto Type() const -> type;
  [[nodiscard]] auto CellId() const -> POINT_2I;

private:

  // type m_type { type::exit };
  POINT_2I m_cellId { 0, 0 };

};

inline portal::portal(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : base_object { position, scale, angle }
{
}

inline auto portal::SetCellId(POINT_2I cellId) -> void
{
  m_cellId = cellId;
}

// inline auto portal::Type() const -> type
// {
//   return m_type;
// }

inline auto portal::CellId() const -> POINT_2I
{
  return m_cellId;
}
