#pragma once

#include "objects/level_asteroid.h"
#include "level_grid.h"

using asteroid_row = std::list<level_asteroid>;
using asteroid_grid = std::list<asteroid_row>;

class asteroid_container;

class asteroid_iterator
{

public:

  friend class asteroid_container;

  enum class type { none, begin, end };

  using row_iterator = asteroid_grid::iterator;
  using column_iterator = asteroid_row::iterator;

  using difference_type = std::ptrdiff_t;
  using value_type = level_asteroid;

  asteroid_iterator() = default;
  auto operator++() -> asteroid_iterator&;
  auto operator++(int) -> asteroid_iterator;
  auto operator*() const -> level_asteroid&;
  auto operator==(const asteroid_iterator& i) const -> bool;

private:
  
  asteroid_iterator(asteroid_container* asteroidContainer, type iteratorType);
  auto EnsureIteratorIsValid() -> void;

  asteroid_container* m_asteroidContainer { nullptr };
  type m_type { type::none };
  row_iterator m_currentRow;
  column_iterator m_currentColumn;
};

class const_asteroid_iterator
{

public:

  friend class asteroid_container;

  enum class type { none, begin, end };

  using row_iterator = asteroid_grid::const_iterator;
  using column_iterator = asteroid_row::const_iterator;

  using difference_type = std::ptrdiff_t;
  using value_type = level_asteroid;

  const_asteroid_iterator() = default;
  auto operator++() -> const_asteroid_iterator&;
  auto operator++(int) -> const_asteroid_iterator;
  auto operator*() const -> const level_asteroid&;
  auto operator==(const const_asteroid_iterator& i) const -> bool;

private:
  
  const_asteroid_iterator(const asteroid_container* asteroidContainer, type iteratorType);
  auto EnsureIteratorIsValid() -> void;

  const asteroid_container* m_asteroidContainer { nullptr };
  type m_type { type::none };
  row_iterator m_currentRow;
  column_iterator m_currentColumn;
};

class asteroid_container
{
public:

  friend class asteroid_iterator;
  friend class const_asteroid_iterator;

  asteroid_container();
  asteroid_container(const asteroid_container&) = default;
  asteroid_container(asteroid_container&&) = default;

  auto Update(const level_grid& grid) -> void;

  auto begin() const -> const_asteroid_iterator;
  auto end() const -> const_asteroid_iterator;

  auto begin() -> asteroid_iterator;
  auto end() -> asteroid_iterator;

private:

  auto CreateAsteroids(const level_grid& grid) -> void;

  level_grid m_smallGrid;
  level_grid m_largeGrid;

  asteroid_grid m_asteroidGrid;
};
