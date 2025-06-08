#pragma once

#include "framework.h"
#include "objects/particle.h"

class const_level_explosion_iterator
{

public:

  friend class level_explosion;

  enum class type { none, begin, end };

  using difference_type = std::ptrdiff_t;
  using value_type = particle;

  const_level_explosion_iterator() = default;

  auto operator++() -> const_level_explosion_iterator&;
  auto operator++(int) -> const_level_explosion_iterator;
  auto operator*() const -> const particle&;
  auto operator==(const const_level_explosion_iterator& i) const -> bool;

private:

  inline static const int m_maxCount { 10 };
  inline static const float m_particleLifespan { 1.5f };

  const_level_explosion_iterator(type iteratorType, D2D1_POINT_2F position);

  static auto GetRandomVelocity() -> VELOCITY_2F;

private:

  inline static std::uniform_int_distribution<int> m_angleDist { 0, 359 };
  inline static std::uniform_int_distribution<int> m_velocityDist { 300, 500 };

  type m_type { type::none };
  D2D1_POINT_2F m_position;
  int m_count { 0 };
  particle m_particle { particle::type::explosion, D2D1_POINT_2F { 0, 0}, { 0, 0 }, 0 };

};

class level_explosion
{

public:

  level_explosion(D2D1_POINT_2F position);

  [[nodiscard]] auto begin() const -> const_level_explosion_iterator;
  [[nodiscard]] auto end() const -> const_level_explosion_iterator;

private:

  D2D1_POINT_2F m_position;

};

inline const_level_explosion_iterator::const_level_explosion_iterator(type iteratorType, D2D1_POINT_2F position) : 
  m_type { iteratorType }, m_position { position }, m_particle { particle::type::explosion, position, GetRandomVelocity(), m_particleLifespan }
{
}

inline auto const_level_explosion_iterator::operator++() -> const_level_explosion_iterator&
{
  if( ++m_count >= m_maxCount )
  {
    m_type = type::end;
  }
  else
  {
    m_particle = particle { particle::type::explosion, m_position, GetRandomVelocity(), m_particleLifespan };
    m_particle.Update(0.1f);
  }

  return *this;
}

inline auto const_level_explosion_iterator::operator++(int) -> const_level_explosion_iterator
{
  auto tmp = *this;

  if( ++m_count >= m_maxCount )
  {
    m_type = type::end;
  }
  else
  {
    m_particle = particle { particle::type::explosion, m_position, GetRandomVelocity(), m_particleLifespan };
  }

  return tmp;
}

inline auto const_level_explosion_iterator::operator*() const -> const particle&
{
  return m_particle;
}

inline auto const_level_explosion_iterator::operator==(const const_level_explosion_iterator& i) const -> bool
{
  if( m_type == type::end && i.m_type == type::end )
  {
    return true;
  }
  else if( m_type == type::end || i.m_type == type::end )
  {
    return false;
  }
  else
  {
    return m_count == i.m_count ? true : false;
  }
}

inline auto const_level_explosion_iterator::GetRandomVelocity() -> VELOCITY_2F
{
  auto angle = static_cast<float>(m_angleDist(pseudo_random_generator::get()));
  auto speed = static_cast<float>(m_velocityDist(pseudo_random_generator::get()));
  return direct2d::CalculateVelocity(speed, angle);
}

inline level_explosion::level_explosion(D2D1_POINT_2F position) : m_position { position }
{
}

inline [[nodiscard]] auto level_explosion::begin() const -> const_level_explosion_iterator
{
  return const_level_explosion_iterator { const_level_explosion_iterator::type::begin, m_position };
}

inline [[nodiscard]] auto level_explosion::end() const -> const_level_explosion_iterator
{
  return const_level_explosion_iterator { const_level_explosion_iterator::type::end, m_position };
}
