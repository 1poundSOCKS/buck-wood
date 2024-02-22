#pragma once

class particle
{

public:

  enum class type { explosion, thrust, impact, bullet_one, bullet_two };

  particle(type particleType, D2D1_POINT_2F position, VELOCITY_2F velocity, float lifespan);

  auto Update(float interval) -> void;
  auto Destroy() -> void;

  [[nodiscard]] auto Type() const -> type { return m_type; }
  [[nodiscard]] auto Age() const -> float { return m_age; }
  [[nodiscard]] auto Lifespan() const -> float { return m_lifespan; }
  [[nodiscard]] auto Position() const -> D2D1_POINT_2F { return m_position; }
  [[nodiscard]] auto Destroyed() const -> bool { return m_destroyed; }

private:

  type m_type;
  D2D1_POINT_2F m_startPosition;
  D2D1_POINT_2F m_position;
  VELOCITY_2F m_velocity;
  float m_lifespan { 0 };
  float m_age { 0 };
  bool m_destroyed { false };

};

inline particle::particle(type particleType, D2D1_POINT_2F position, VELOCITY_2F velocity, float lifespan) :
  m_type { particleType }, m_startPosition { position }, m_position { position } , m_velocity { velocity }, m_lifespan { lifespan }
{
}

inline auto particle::Update(float interval) -> void
{
  m_position = direct2d::CalculatePosition(m_position, m_velocity, interval);
  m_age += interval;
  m_destroyed = m_age < m_lifespan ? false : true;
}

inline auto particle::Destroy() -> void
{
  m_destroyed = true;
}
