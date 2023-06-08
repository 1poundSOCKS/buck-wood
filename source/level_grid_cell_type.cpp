#include "pch.h"
#include "level_grid_cell_type.h"
#include "perlin_simplex_noise.h"

level_grid_cell_type::level_grid_cell_type(const level_grid_cell& cell)
{
  auto noise = psn::GetNoise(static_cast<float>(cell.x) / 13, static_cast<float>(cell.y) / 13);

  if( noise > 0.87 )
  {
    m_type = type::asteroid;
  }
  else if( noise < -0.96 )
  {
    m_type = type::target;
  }
  else if( noise < -0.85 )
  {
    m_type = type::mine;
  }
}

[[nodiscard]] auto level_grid_cell_type::Get() const -> type
{
  return m_type;
}

[[nodiscard]] auto level_grid_cell_type::IsAsteroid() const -> bool
{
  return m_type == type::asteroid;
}

[[nodiscard]] auto level_grid_cell_type::IsTarget() const -> bool
{
  return m_type == type::target;
}

[[nodiscard]] auto level_grid_cell_type::IsMine() const -> bool
{
  return m_type == type::mine;
}
