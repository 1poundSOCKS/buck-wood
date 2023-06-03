#pragma once

#include "geometry.h"
#include "level_asteroid.h"
#include "level_grid.h"

class asteroid_container;

class asteroid_iterator
{

public:

  friend class asteroid_container;

  enum class type { none, begin, end };

  using asteroid_collection = std::list<level_asteroid>;
  using iterator = asteroid_iterator::asteroid_collection::iterator;

  using difference_type = std::ptrdiff_t;
  using value_type = level_asteroid;

  asteroid_iterator() = default;
  auto operator++() -> asteroid_iterator&;
  auto operator++(int) -> asteroid_iterator;
  auto operator*() const -> level_asteroid&;
  auto operator==(const asteroid_iterator& i) const -> bool;
  // auto operator<=>(const asteroid_iterator& i) const -> std::strong_ordering;

private:
  
  asteroid_iterator(asteroid_container* asteroidContainer, type iteratorType);

  asteroid_container* m_asteroidContainer { nullptr };
  type m_type { type::none };
  iterator m_currentAsteroid;
};

class const_asteroid_iterator
{

public:

  friend class asteroid_container;

  enum class type { none, begin, end };

  using asteroid_collection = std::list<level_asteroid>;
  using iterator = asteroid_iterator::asteroid_collection::const_iterator;

  using difference_type = std::ptrdiff_t;
  using value_type = level_asteroid;

  const_asteroid_iterator() = default;
  auto operator++() -> const_asteroid_iterator&;
  auto operator++(int) -> const_asteroid_iterator;
  auto operator*() const -> const level_asteroid&;
  auto operator==(const const_asteroid_iterator& i) const -> bool;
  // auto operator<=>(const const_asteroid_iterator& i) const -> std::strong_ordering;

private:
  
  const_asteroid_iterator(const asteroid_container* asteroidContainer, type iteratorType);

  const asteroid_container* m_asteroidContainer { nullptr };
  type m_type { type::none };
  iterator m_currentAsteroid;
};

class asteroid_container
{
public:

  friend class asteroid_iterator;
  friend class const_asteroid_iterator;

  asteroid_container() = default;
  asteroid_container(const asteroid_container&) = default;
  asteroid_container(asteroid_container&&) = default;

  auto Update(const D2D1_RECT_F& rect) -> void;

  auto begin() const -> const_asteroid_iterator;
  auto end() const -> const_asteroid_iterator;

  auto begin() -> asteroid_iterator;
  auto end() -> asteroid_iterator;

private:

  auto CreateSmallAsteroids(const level_grid& grid, auto inserter) -> void;
  auto CreateLargeAsteroids(const level_grid& grid, auto inserter) -> void;

  level_grid m_smallGrid;
  level_grid m_largeGrid;

  asteroid_iterator::asteroid_collection m_smallAsteroids;
  asteroid_iterator::asteroid_collection m_largeAsteroids;
  
};
