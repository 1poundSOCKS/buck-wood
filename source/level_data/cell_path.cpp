#include "pch.h"
#include "cell_path.h"

cell_path::const_iterator::const_iterator(const cell_path& c, cell_id cellId) : m_container { c }, m_cellId { cellId }
{
}

auto cell_path::const_iterator::operator++() -> cell_path::const_iterator &
{
  m_cellId = m_container.Next(m_cellId);
  return *this;
}

auto cell_path::const_iterator::operator++(int) -> cell_path::const_iterator
{
  const_iterator thisCopy { m_container, m_cellId };
  m_cellId = m_container.Next(m_cellId);
  return thisCopy;
}

auto cell_path::const_iterator::operator*() const -> const cell_id &
{
  return m_cellId;
}

auto cell_path::const_iterator::operator==(const cell_path::const_iterator &i) const -> bool
{
  return m_cellId == i.m_cellId;
}

cell_path::cell_path(cell_id begin, cell_id end) : m_begin { begin }, m_end { end }
{
}

auto cell_path::begin() const -> const_iterator
{
  return const_iterator(*this, m_begin);
}

auto cell_path::end() const -> const_iterator
{
  return const_iterator(*this, m_end);
}

auto cell_path::Next(cell_id cellId) const noexcept -> cell_id
{
  auto position = cellId.Position();
  auto endPosition = m_end.Position();
  auto distanceToEnd = POINT_2I { endPosition.x - position.x, endPosition.y - position.y };
  auto absoluteDistanceToEnd = POINT_2I { std::abs(distanceToEnd.x), std::abs(distanceToEnd.y) };

  if( absoluteDistanceToEnd.x > absoluteDistanceToEnd.y )
  {
    auto positionOffset = distanceToEnd.x > 0 ? 1 : -1;
    auto nextPosition = POINT_2I { position.x + positionOffset, position.y };
    return { nextPosition.x, nextPosition.y };
  }
  else
  {
    auto positionOffset = distanceToEnd.y > 0 ? 1 : -1;
    auto nextPosition = POINT_2I { position.x, position.y + positionOffset};
    return { nextPosition.x, nextPosition.y };
  }
}
