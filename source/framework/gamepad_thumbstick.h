#pragma once

#include "geometry.h"

class gamepad_thumbstick
{

public:

  gamepad_thumbstick(SHORT lx, SHORT ly);

  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Distance() const -> float;

private:

  SHORT m_lx { 0 };
  SHORT m_ly { 0 };

};

inline gamepad_thumbstick::gamepad_thumbstick(SHORT lx, SHORT ly) : m_lx { lx }, m_ly { ly }
{
}

[[nodiscard]] inline auto gamepad_thumbstick::Angle() const -> float
{
  return game_point { 0, 0 }.AngleTo(game_point { static_cast<float>(m_lx), static_cast<float>(-m_ly) });
}

[[nodiscard]] inline auto gamepad_thumbstick::Distance() const -> float
{
  return static_cast<float>(sqrt(m_lx * m_lx + m_ly * m_ly));
}
