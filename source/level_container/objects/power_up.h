#pragma once

class power_up
{

public:

  power_up(POINT_2F position) : m_position { position }
  {
  }

  constexpr [[nodiscard]] auto Scale() const -> SCALE_2F { return { 1.0f, 1.0f }; };
  constexpr [[nodiscard]] auto Angle() const -> float { return 0; }

  [[nodiscard]] auto Position() const -> POINT_2F
  {
    return m_position;
  }

  [[nodiscard]] auto Destroyed() const -> bool
  {
    return m_destroyed;
  }

  auto Update(float interval) -> void
  {
  }

  auto Destroy() -> void
  {
    m_destroyed = true;
  }

private:

  POINT_2F m_position;
  bool m_destroyed { false };

};
