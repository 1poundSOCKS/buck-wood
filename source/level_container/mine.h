#pragma once

#include "framework.h"
#include "directional_body.h"

class mine
{

public:

  mine(float x, float y);

  [[nodiscard]] auto Position() const -> const game_point&;
  [[nodiscard]] auto PreviousPosition() const -> const game_point&;
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Destroyed() const -> bool;

  auto Destroy() -> void;
  auto Update(float interval, float x, float y) -> void;
  auto Update(float interval) -> void;

private:

  inline static const float m_spinRate { 500.0f };
  inline static const float m_thrustPower { 100.0f };
  directional_body m_body;
  directional_body m_previousState;
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
