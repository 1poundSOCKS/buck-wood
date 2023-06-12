#include "pch.h"
#include "asteroid_container.h"
#include "level_grid_cell_type.h"

asteroid_iterator::asteroid_iterator(asteroid_container* asteroidContainer, type iteratorType) : m_asteroidContainer { asteroidContainer }, m_type { iteratorType }
{
  switch( iteratorType )
  {
    case type::begin:
      m_currentRow = std::begin(asteroidContainer->m_asteroidGrid);
      m_currentColumn = m_currentRow->begin();
      EnsureIteratorIsValid();
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

  EnsureIteratorIsValid();

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

auto asteroid_iterator::EnsureIteratorIsValid() -> void
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
      EnsureIteratorIsValid();
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

  EnsureIteratorIsValid();

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

auto const_asteroid_iterator::EnsureIteratorIsValid() -> void
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

auto asteroid_container::Update(const level_grid& grid) -> void
{
  m_asteroidGrid.front().clear();
  CreateAsteroids(grid);
}

auto asteroid_container::CreateAsteroids(const level_grid& grid) -> void
{
  auto asteroidView = grid | std::ranges::views::filter([](const auto& cell)
  {
    auto cellType = level_grid_cell_type { cell };
    return cellType.IsAsteroid();
  });

  std::ranges::for_each(asteroidView, [this](auto cell)
  {
    cell.Resize(0.6f, 0.6f);

    m_asteroidGrid.front().emplace_back(level_asteroid { game_rect
    { 
      { cell.Left(), cell.Top() } , 
      { cell.Right(), cell.Bottom() }
    }});
  });

  m_smallGrid = grid;
}
