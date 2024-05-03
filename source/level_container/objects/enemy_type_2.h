#pragma once

#include "enemy_object.h"
#include "reload_timer.h"
#include "level_cell_collection.h"

class enemy_type_2 : public enemy_object
{

public:

  enemy_type_2(POINT_2F position, int hitpoints, float waitTime, float speed, float reloadTime);

  auto Update(float interval, const level_cell_collection& cells) -> void;

  [[nodiscard]] auto Destination() const -> std::optional<valid_cell>;
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;

private:

  enum class status { moving, waiting };

  [[nodiscard]] auto UpdateWhenMoving(float interval, const level_cell_collection& cells) noexcept -> status;
  [[nodiscard]] auto UpdateWhenWaiting(float interval) noexcept -> status;
  auto MoveTowardsDestination(valid_cell destination, float interval) noexcept -> bool;
  [[nodiscard]] auto NewDestination(const level_cell_collection& cells) -> std::optional<valid_cell>;

private:

  status m_status;
  reload_timer m_waitTimer;
  float m_speed;
  std::optional<valid_cell> m_destination;
  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  // std::shared_ptr<valid_cell_collection> m_cells;

};

inline [[nodiscard]] auto enemy_type_2::Destination() const -> std::optional<valid_cell>
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
