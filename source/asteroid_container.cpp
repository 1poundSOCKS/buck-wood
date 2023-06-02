#include "pch.h"
#include "asteroid_container.h"
#include "game_level_object_generator.h"

auto asteroid_container::begin() const -> asteroid_collection::const_iterator
{
  return std::cbegin(m_asteroids);
}

auto asteroid_container::end() const -> asteroid_collection::const_iterator
{
  return std::cend(m_asteroids);
}

auto asteroid_container::Update(const D2D1_RECT_F& rect) -> void
{
  auto smallColumnWidth = 100;
  auto smallRowHeight = 100;
  auto smallGrid = level_grid { smallColumnWidth, smallRowHeight, static_cast<int>(rect.left), static_cast<int>(rect.top), static_cast<int>(rect.right), static_cast<int>(rect.bottom) };

  auto largeColumnWidth = 400;
  auto largeRowHeight = 400;
  auto largeGrid = level_grid { largeColumnWidth, largeRowHeight, static_cast<int>(rect.left), static_cast<int>(rect.top), static_cast<int>(rect.right), static_cast<int>(rect.bottom) };

  if( m_smallGrid != smallGrid || m_largeGrid != largeGrid )
  {
    m_asteroids.clear();
    CreateSmallAsteroids(smallGrid, std::back_inserter(m_asteroids));
    CreateLargeAsteroids(largeGrid, std::back_inserter(m_asteroids));
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
