#pragma once

class level_input
{
public:

  level_input(std::optional<float> angle, float thrust, float shoot);

  [[nodiscard]] auto Angle() const -> const std::optional<float>&;
  [[nodiscard]] auto Thrust() const -> float;
  [[nodiscard]] auto Shoot() const -> float;

private:

  std::optional<float> m_angle;
  float m_thrust { 0 };
  float m_shoot { 0 };
};

inline level_input::level_input(std::optional<float> angle, float thrust, float shoot) : m_angle { angle }, m_thrust { thrust }, m_shoot { shoot }
{
}

[[nodiscard]] inline auto level_input::Angle() const -> const std::optional<float>&
{
  return m_angle;
}

[[nodiscard]] inline auto level_input::Thrust() const -> float
{
  return m_thrust;
}

[[nodiscard]] inline auto level_input::Shoot() const -> float
{
  return m_shoot;
}
