#pragma once

#include "framework.h"
#include "homing_object.h"
#include "game_scale.h"

class mine
{

public:

  mine(float x, float y);

  [[nodiscard]] auto Scale() const -> game_scale { return game_scale { 1.0f, 1.0f }; };
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> const game_point&;
  [[nodiscard]] auto Destroyed() const -> bool;

  auto Update(float interval, const std::optional<game_point>& playerPosition) -> void;
  auto Destroy() -> void;

  [[nodiscard]] auto PreviousPosition() const -> const game_point&;

private:

  inline static const float m_spinRate { 500.0f };
  inline static const float m_thrustPower { 100.0f };
  homing_object m_body;
  homing_object m_previousState;
  game_angle m_spin;
  bool m_destroyed { false };

};

inline [[nodiscard]] auto mine::Position() const -> const game_point&
{
  return m_body.Position();
}

inline [[nodiscard]] auto mine::PreviousPosition() const -> const game_point&
{
  return m_previousState.Position();
}

inline [[nodiscard]] auto mine::Angle() const -> float
{
  return m_spin;
}
