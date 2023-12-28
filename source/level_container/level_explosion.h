#pragma once

#include "framework.h"
#include "objects/explosion_particle.h"

class const_level_explosion_iterator
{

public:

  friend class level_explosion;

  enum class type { none, begin, end };

  using difference_type = std::ptrdiff_t;
  using value_type = explosion_particle;

  const_level_explosion_iterator() = default;

  auto operator++() -> const_level_explosion_iterator&;
  auto operator++(int) -> const_level_explosion_iterator;
  auto operator*() const -> const explosion_particle&;
  auto operator==(const const_level_explosion_iterator& i) const -> bool;

private:

  inline static const int m_maxCount { 50 };

  const_level_explosion_iterator(type iteratorType, const game_point& position);

  static auto GetRandomVelocity() -> game_velocity;

private:

  inline static std::uniform_int_distribution<int> m_angleDist { 0, 359 };
  inline static std::uniform_int_distribution<int> m_velocityDist { 200, 300 };

  type m_type { type::none };
  game_point m_position;
  int m_count { 0 };
  explosion_particle m_particle { game_point { 0, 0}, game_velocity{ 0, 0 }, 0 };

};

class level_explosion
{

public:

  level_explosion(const game_point& position);

  [[nodiscard]] auto begin() const -> const_level_explosion_iterator;
  [[nodiscard]] auto end() const -> const_level_explosion_iterator;

private:

  game_point m_position;

};

inline const_level_explosion_iterator::const_level_explosion_iterator(type iteratorType, const game_point& position) : 
  m_type { iteratorType }, m_position { position }, m_particle { position, GetRandomVelocity(), 5 }
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
    m_particle = explosion_particle { m_position, GetRandomVelocity(), 5 };
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
    m_particle = explosion_particle { m_position, GetRandomVelocity(), 5 };
  }

  return tmp;
}

inline auto const_level_explosion_iterator::operator*() const -> const explosion_particle&
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

inline auto const_level_explosion_iterator::GetRandomVelocity() -> game_velocity
{
  auto angle = static_cast<float>(m_angleDist(pseudo_random_generator::get()));
  auto velocity = static_cast<float>(m_velocityDist(pseudo_random_generator::get()));
  return { game_angle { angle }, velocity };
}

inline level_explosion::level_explosion(const game_point& position) : m_position { position }
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
