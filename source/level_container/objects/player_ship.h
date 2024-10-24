#pragma once

#include "framework.h"
#include "base_object.h"
#include "play_event.h"
#include "reload_timer.h"
#include "player_ship_state.h"

class player_ship
{

public:

  player_ship(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity);

  auto Update(VELOCITY_2F environmentalForces, float airResistance, float interval) -> void;

  [[nodiscard]] auto Position() const noexcept -> POINT_2F;
  [[nodiscard]] auto Scale() const noexcept -> SCALE_2F;
  [[nodiscard]] auto Angle() const noexcept -> float;
  [[nodiscard]] auto Age() const noexcept -> float;
  [[nodiscard]] auto Destroyed() const noexcept -> bool;
  auto Destroy() noexcept -> void;

  [[nodiscard]] auto State() const -> std::shared_ptr<player_ship_state>;
  [[nodiscard]] auto StateValue() const -> player_ship_state;

private:

  std::shared_ptr<player_ship_state> m_state;

};
