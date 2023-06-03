#include "pch.h"
#include "asteroid_container.h"
#include "game_level_object_generator.h"

asteroid_iterator::asteroid_iterator(asteroid_container* asteroidContainer, type iteratorType) : m_asteroidContainer { asteroidContainer }, m_type { iteratorType }
{
  switch( iteratorType )
  {
    case type::begin:
      m_currentAsteroid = std::begin(asteroidContainer->m_smallAsteroids);
      break;
    case type::end:
      m_currentAsteroid = std::end(asteroidContainer->m_smallAsteroids);
      break;
  }
}

auto asteroid_iterator::operator++() -> asteroid_iterator&
{
  ++m_currentAsteroid;
  return *this;
}

auto asteroid_iterator::operator++(int) -> asteroid_iterator
{
  asteroid_iterator tmp = *this;
  m_currentAsteroid++;
  return tmp;
}

auto asteroid_iterator::operator*() const -> level_asteroid&
{
  return *m_currentAsteroid;
}

auto asteroid_iterator::operator==(const asteroid_iterator& i) const -> bool
{
  return m_currentAsteroid == i.m_currentAsteroid;
}

// auto asteroid_iterator::operator<=>(const asteroid_iterator& i) const -> std::strong_ordering
// {
//   return std::strong_ordering::equal;
// }

const_asteroid_iterator::const_asteroid_iterator(const asteroid_container* asteroidContainer, type iteratorType) : 
  m_asteroidContainer { asteroidContainer }, m_type { iteratorType }
{
  switch( iteratorType )
  {
    case type::begin:
      m_currentAsteroid = std::begin(asteroidContainer->m_smallAsteroids);
      break;
    case type::end:
      m_currentAsteroid = std::end(asteroidContainer->m_smallAsteroids);
      break;
  }
}

auto const_asteroid_iterator::operator++() -> const_asteroid_iterator&
{
  ++m_currentAsteroid;
  return *this;
}

auto const_asteroid_iterator::operator++(int) -> const_asteroid_iterator
{
  const_asteroid_iterator tmp = *this;
  m_currentAsteroid++;
  return tmp;
}

auto const_asteroid_iterator::operator*() const -> const level_asteroid&
{
  return *m_currentAsteroid;
}

auto const_asteroid_iterator::operator==(const const_asteroid_iterator& i) const -> bool
{
  return m_currentAsteroid == i.m_currentAsteroid;
}

// auto const_asteroid_iterator::operator<=>(const const_asteroid_iterator& i) const -> std::strong_ordering
// {
//   return std::strong_ordering::equal;
// }

auto asteroid_container::begin() const -> const_asteroid_iterator
{
  return const_asteroid_iterator { this, const_asteroid_iterator::type::begin };
}

auto asteroid_container::end() const -> const_asteroid_iterator
{
  return const_asteroid_iterator { this, const_asteroid_iterator::type::end };
}

auto asteroid_container::begin() -> asteroid_iterator
{
  return asteroid_iterator { this, asteroid_iterator::type::begin };
}

auto asteroid_container::end() -> asteroid_iterator
{
  return asteroid_iterator { this, asteroid_iterator::type::end };
}

auto asteroid_container::Update(const D2D1_RECT_F& rect) -> void
{
  auto smallColumnWidth = 100;
  auto smallRowHeight = 100;
  auto smallGrid = level_grid { smallColumnWidth, smallRowHeight, static_cast<int>(rect.left), static_cast<int>(rect.top), static_cast<int>(rect.right), static_cast<int>(rect.bottom) };

  auto largeColumnWidth = 400;
  auto largeRowHeight = 400;
  auto largeGrid = level_grid { largeColumnWidth, largeRowHeight, static_cast<int>(rect.left), static_cast<int>(rect.top), static_cast<int>(rect.right), static_cast<int>(rect.bottom) };

  if( m_smallGrid != smallGrid )
  {
    m_smallAsteroids.clear();
    CreateSmallAsteroids(smallGrid, std::back_inserter(m_smallAsteroids));
  }

  if( m_largeGrid != largeGrid )
  {
    m_largeAsteroids.clear();
    CreateLargeAsteroids(largeGrid, std::back_inserter(m_largeAsteroids));
  }
}

auto asteroid_container::CreateSmallAsteroids(const level_grid& grid, auto inserter) -> void
{
  game_level_object_generator smallAsteroidGenerator(grid.LeftColumn(), grid.RightColumn(), grid.ColumnWidth(), grid.TopRow(), grid.BottomRow(), grid.RowHeight(), 0.85f, 0.89f, 13.0f);
  smallAsteroidGenerator.InsertInto(inserter);
  m_smallGrid = grid;
}

auto asteroid_container::CreateLargeAsteroids(const level_grid& grid, auto inserter) -> void
{
  game_level_object_generator largeAsteroidGenerator(grid.LeftColumn(), grid.RightColumn(), grid.ColumnWidth(), grid.TopRow(), grid.BottomRow(), grid.RowHeight(), 0.90f, 1.0f, 7.0f);
  largeAsteroidGenerator.InsertInto(inserter);
  m_largeGrid = grid;
}
