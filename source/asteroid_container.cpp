#include "pch.h"
#include "asteroid_container.h"
#include "game_level_object_generator.h"

asteroid_iterator::asteroid_iterator(asteroid_container* asteroidContainer, type iteratorType) : m_asteroidContainer { asteroidContainer }, m_type { iteratorType }
{
  switch( iteratorType )
  {
    case type::begin:
      m_currentRow = std::begin(asteroidContainer->m_asteroidGrid);
      m_currentColumn = m_currentRow->begin();
      EnsureCurrentColumnIsValid();
      break;
  }
}

auto asteroid_iterator::operator++() -> asteroid_iterator&
{
  ++m_currentColumn;

  while( m_type != type::end && m_currentColumn == m_currentRow->end() )
  {
    ++m_currentRow;

    if( m_currentRow == std::end(m_asteroidContainer->m_asteroidGrid) )
    {
      m_type = type::end;
    }
    else
    {
      m_currentColumn = m_currentRow->begin();
    }
  }

  return *this;
}

auto asteroid_iterator::operator++(int) -> asteroid_iterator
{
  asteroid_iterator tmp = *this;
  
  m_currentColumn++;

  EnsureCurrentColumnIsValid();

  return tmp;
}

auto asteroid_iterator::operator*() const -> level_asteroid&
{
  return *m_currentColumn;
}

auto asteroid_iterator::operator==(const asteroid_iterator& i) const -> bool
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
    return m_currentRow == i.m_currentRow && m_currentColumn == i.m_currentColumn;
  }
}

auto asteroid_iterator::EnsureCurrentColumnIsValid() -> void
{
  while( m_type != type::end && m_currentColumn == m_currentRow->end() )
  {
    m_currentRow++;

    if( m_currentRow == std::end(m_asteroidContainer->m_asteroidGrid) )
    {
      m_type = type::end;
    }
    else
    {
      m_currentColumn = m_currentRow->begin();
    }
  }
}

const_asteroid_iterator::const_asteroid_iterator(const asteroid_container* asteroidContainer, type iteratorType) : 
  m_asteroidContainer { asteroidContainer }, m_type { iteratorType }
{
  switch( iteratorType )
  {
    case type::begin:
      m_currentRow = std::cbegin(asteroidContainer->m_asteroidGrid);
      m_currentColumn = m_currentRow->cbegin();
      EnsureCurrentColumnIsValid();
      break;
  }
}

auto const_asteroid_iterator::operator++() -> const_asteroid_iterator&
{
  ++m_currentColumn;

  while( m_type != type::end && m_currentColumn == m_currentRow->cend() )
  {
    ++m_currentRow;

    if( m_currentRow == std::end(m_asteroidContainer->m_asteroidGrid) )
    {
      m_type = type::end;
    }
    else
    {
      m_currentColumn = m_currentRow->cbegin();
    }
  }

  return *this;
}

auto const_asteroid_iterator::operator++(int) -> const_asteroid_iterator
{
  const_asteroid_iterator tmp = *this;

  m_currentColumn++;

  EnsureCurrentColumnIsValid();

  return tmp;
}

auto const_asteroid_iterator::operator*() const -> const level_asteroid&
{
  return *m_currentColumn;
}

auto const_asteroid_iterator::operator==(const const_asteroid_iterator& i) const -> bool
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
    return m_currentRow == i.m_currentRow && m_currentColumn == i.m_currentColumn;
  }
}

auto const_asteroid_iterator::EnsureCurrentColumnIsValid() -> void
{
  while( m_type != type::end && m_currentColumn == m_currentRow->cend() )
  {
    m_currentRow++;

    if( m_currentRow == std::end(m_asteroidContainer->m_asteroidGrid) )
    {
      m_type = type::end;
    }
    else
    {
      m_currentColumn = m_currentRow->cbegin();
    }
  }
}

asteroid_container::asteroid_container()
{
  m_asteroidGrid.resize(2);
}

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
    m_asteroidGrid.front().clear();
    CreateSmallAsteroids(smallGrid, std::back_inserter(m_asteroidGrid.front()));
  }

  if( m_largeGrid != largeGrid )
  {
    m_asteroidGrid.back().clear();
    CreateLargeAsteroids(largeGrid, std::back_inserter(m_asteroidGrid.back()));
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
