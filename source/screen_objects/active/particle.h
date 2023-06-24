#pragma once

#include "moving_body.h"

class particle
{

public:

  particle() = default;
  particle(const game_point& position, const game_velocity& velocity, float lifespan);

  auto Update(int64_t ticks) -> void;
  auto Update(float interval) -> void;

  [[nodiscard]] auto Age() const -> float;
  [[nodiscard]] auto Lifespan() const -> float;
  [[nodiscard]] auto Position() const -> const game_point&;
  [[nodiscard]] auto Destroyed() const -> bool;

private:

  game_point m_startPosition;
  moving_body m_movingBody;
  float m_lifespan { 0 };
  float m_age { 0 };
  bool m_destroyed { false };

};

inline particle::particle(const game_point& position, const game_velocity& velocity, float lifespan) :
  m_startPosition { position }, m_movingBody { position, velocity }, m_lifespan { lifespan }
{
}

inline auto particle::Update(int64_t ticks) -> void
{
  Update(framework::gameUpdateInterval(ticks));
}

inline auto particle::Update(float interval) -> void
{
  m_movingBody.Update(interval);

  m_age += interval;

  m_destroyed = m_age < m_lifespan ? false : true;
}

[[nodiscard]] inline auto particle::Age() const -> float
{
  return m_age;
}

[[nodiscard]] inline auto particle::Lifespan() const -> float
{
  return m_lifespan;
}

[[nodiscard]] inline auto particle::Position() const -> const game_point&
{
  return m_movingBody.Position();
}

[[nodiscard]] inline auto particle::Destroyed() const -> bool
{
  return m_destroyed;
}
