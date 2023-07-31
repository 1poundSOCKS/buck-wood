#pragma once

class level_input
{
public:

  level_input(std::optional<float> angle, float thrust, std::optional<float> shootAngle);

  [[nodiscard]] auto Angle() const -> const std::optional<float>&;
  [[nodiscard]] auto Thrust() const -> float;
  [[nodiscard]] auto ShootAngle() const -> const std::optional<float>&;

private:

  std::optional<float> m_angle;
  float m_thrust { 0 };
  std::optional<float> m_shootAngle;
};

inline level_input::level_input(std::optional<float> angle, float thrust, std::optional<float> shootAngle) : m_angle { angle }, m_thrust { thrust }, m_shootAngle { shootAngle }
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

[[nodiscard]] inline auto level_input::ShootAngle() const -> const std::optional<float>&
{
  return m_shootAngle;
}
