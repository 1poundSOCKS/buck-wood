#pragma once

#include "enemy_object.h"
#include "reload_timer.h"
#include "valid_cell_collection.h"

class enemy_type_2 : public enemy_object
{

public:

  enemy_type_2(POINT_2F position, int hitpoints, float speed, float reloadTime, valid_cell_collection cells);

  auto Update(float interval) -> void;

  [[nodiscard]] auto Destination() const -> std::optional<POINT_2F>;
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;

private:

  [[nodiscard]] auto NewDestination() -> POINT_2F;

private:

  float m_speed;
  std::optional<POINT_2F> m_destination;
  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  valid_cell_collection m_cells;

};

inline [[nodiscard]] auto enemy_type_2::Destination() const -> std::optional<POINT_2F>
{
  return m_destination;
}

inline [[nodiscard]] auto enemy_type_2::CanShootAt(POINT_2F position) const -> bool
{
  return m_reloaded;
}

inline [[nodiscard]] auto enemy_type_2::Reloaded() const -> bool
{
  return m_reloaded;
}
