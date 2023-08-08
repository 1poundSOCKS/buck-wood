#pragma once

#include "geometry.h"

class gamepad_thumbstick
{

public:

  gamepad_thumbstick(SHORT lx, SHORT ly);

  [[nodiscard]] auto X() const -> SHORT;
  [[nodiscard]] auto Y() const -> SHORT;

  [[nodiscard]] auto XRatio() const -> float;
  [[nodiscard]] auto YRatio() const -> float;

  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Distance() const -> float;

private:

  [[nodiscard]] static auto Ratio(SHORT value) -> float;

  SHORT m_x { 0 };
  SHORT m_y { 0 };

};

inline gamepad_thumbstick::gamepad_thumbstick(SHORT lx, SHORT ly) : m_x { lx }, m_y { ly }
{
}

[[nodiscard]] inline auto gamepad_thumbstick::X() const -> SHORT
{
  return m_x;
}

[[nodiscard]] inline auto gamepad_thumbstick::Y() const -> SHORT
{
  return m_y;
}

[[nodiscard]] inline auto gamepad_thumbstick::XRatio() const -> float
{
  return Ratio(m_x);
}

[[nodiscard]] inline auto gamepad_thumbstick::YRatio() const -> float
{
  return Ratio(m_y);
}

[[nodiscard]] inline auto gamepad_thumbstick::Ratio(SHORT value) -> float
{
  constexpr auto positiveShift = -SHRT_MIN;
  auto positiveShiftedValue = static_cast<float>(value) + positiveShift;
  return positiveShiftedValue / positiveShift - 1.0f;
}

[[nodiscard]] inline auto gamepad_thumbstick::Angle() const -> float
{
  return game_point { 0, 0 }.AngleTo(game_point { static_cast<float>(m_x), static_cast<float>(-m_y) });
}

[[nodiscard]] inline auto gamepad_thumbstick::Distance() const -> float
{
  return static_cast<float>(sqrt(m_x * m_x + m_y * m_y));
}
