#pragma once

#include "moving_body.h"

class particle
{

public:

  particle(D2D1_POINT_2F position, VELOCITY_2F velocity, float lifespan);

  auto Update(float interval) -> void;
  auto Destroy() -> void;

  [[nodiscard]] auto Age() const -> float;
  [[nodiscard]] auto Lifespan() const -> float;
  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Destroyed() const -> bool;

private:

  D2D1_POINT_2F m_startPosition;
  moving_body m_movingBody;
  float m_lifespan { 0 };
  float m_age { 0 };
  bool m_destroyed { false };

};

inline particle::particle(D2D1_POINT_2F position, VELOCITY_2F velocity, float lifespan) :
  m_startPosition { position }, m_movingBody { position, velocity }, m_lifespan { lifespan }
{
}

inline auto particle::Update(float interval) -> void
{
  m_movingBody.Update(interval);

  m_age += interval;

  m_destroyed = m_age < m_lifespan ? false : true;
}

inline auto particle::Destroy() -> void
{
  m_destroyed = true;
}

[[nodiscard]] inline auto particle::Age() const -> float
{
  return m_age;
}

[[nodiscard]] inline auto particle::Lifespan() const -> float
{
  return m_lifespan;
}

[[nodiscard]] inline auto particle::Position() const -> D2D1_POINT_2F
{
  return m_movingBody.Position();
}

[[nodiscard]] inline auto particle::Destroyed() const -> bool
{
  return m_destroyed;
}
