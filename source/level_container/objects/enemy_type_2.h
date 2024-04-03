#pragma once

#include "enemy_object.h"
#include "reload_timer.h"

class enemy_type_2 : public enemy_object
{

public:

  enum class state_type { moving, shooting };

  enemy_type_2(POINT_2F position);
  enemy_type_2(POINT_2F position, float reloadTime, int hitpoints);

  auto Update(float interval) -> void;

  [[nodiscard]] auto State() const -> state_type;
  [[nodiscard]] auto Destination() const -> std::optional<POINT_2F>;
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;

private:

  state_type m_state { state_type::moving };
  constexpr static int m_movesBeforeStateChange { 3 };
  constexpr static float m_speed = 300;
  inline static std::uniform_int_distribution<int> m_positionDist { -10, 10 };

  std::optional<POINT_2F> m_destination;
  reload_timer m_reloadTimer;
  bool m_reloaded { false };
  constexpr static int m_shotsBeforeStateChange { 5 };

  int m_actionCount { 0 };

};

inline [[nodiscard]] auto enemy_type_2::State() const -> state_type
{
  return m_state;
}

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
