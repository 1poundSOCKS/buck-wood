#pragma once

class level_input
{
public:

  level_input(std::optional<float> angle, std::optional<float> rotation, float thrust, std::optional<float> shootAngle);

  [[nodiscard]] auto Angle() const -> const std::optional<float>&;
  [[nodiscard]] auto Rotation() const -> const std::optional<float>&;
  [[nodiscard]] auto Thrust() const -> float;
  [[nodiscard]] auto ShootAngle() const -> const std::optional<float>&;

private:

  std::optional<float> m_angle;
  std::optional<float> m_rotation;
  float m_thrust { 0 };
  std::optional<float> m_shootAngle;
};

inline level_input::level_input(std::optional<float> angle, std::optional<float> rotation, float thrust, std::optional<float> shootAngle) : 
  m_angle { angle }, m_rotation { rotation }, m_thrust { thrust }, m_shootAngle { shootAngle }
{
}

[[nodiscard]] inline auto level_input::Angle() const -> const std::optional<float>&
{
  return m_angle;
}

[[nodiscard]] inline auto level_input::Rotation() const -> const std::optional<float>&
{
  return m_rotation;
}

[[nodiscard]] inline auto level_input::Thrust() const -> float
{
  return m_thrust;
}

[[nodiscard]] inline auto level_input::ShootAngle() const -> const std::optional<float>&
{
  return m_shootAngle;
}
