#pragma once

#include "framework.h"
#include "game_angle.h"

struct game_velocity
{
  game_velocity() = default;
  game_velocity(float direction, float speed);

  auto operator+=(const game_velocity& increase) -> game_velocity&;
  [[nodiscard]] auto operator+(const game_velocity& increase) const -> game_velocity;
  auto Update(float xMultiplier, float yMultipler) -> void;

  [[nodiscard]] auto x() const -> float;
  [[nodiscard]] auto y() const -> float;
  [[nodiscard]] auto Speed() const -> float;
  [[nodiscard]] auto Direction() const -> float;

  auto Accelerate(float cx, float cy) -> void;

private:

  float m_x { 0 };
  float m_y { 0 };
};

inline game_velocity::game_velocity(float direction, float speed)
{
  m_x = speed * sin(DEGTORAD(direction));
  m_y = -speed * cos(DEGTORAD(direction));
}

inline auto game_velocity::operator+=(const game_velocity& increase) -> game_velocity&
{
  m_x += increase.m_x;
  m_y += increase.m_y;
  return *this;
}

inline [[nodiscard]] auto game_velocity::operator+(const game_velocity& value) const -> game_velocity
{
  return { m_x + value.m_x, m_y + value.m_y };
}

inline auto game_velocity::Update(float xMultiplier, float yMultiplier) -> void
{
  m_x *= xMultiplier;
  m_y *= yMultiplier;
}

inline [[nodiscard]] auto game_velocity::x() const -> float
{
  return m_x;
}

inline [[nodiscard]] auto game_velocity::y() const -> float
{
  return m_y;
}

inline [[nodiscard]] auto game_velocity::Speed() const -> float
{
  return sqrt(m_x * m_x + m_y * m_y);
}

inline [[nodiscard]] auto game_velocity::Direction() const -> float
{
  return direct2d::GetAngleBetween({0, 0}, {m_x, m_y});
}

inline auto game_velocity::Accelerate(float cx, float cy) -> void
{
  m_x += cx;
  m_y += cy;
}
