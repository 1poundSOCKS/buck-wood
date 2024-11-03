#include "pch.h"
#include "cell_path.h"

cell_path::const_iterator::const_iterator(const cell_path* c, POINT_2I cellId) : m_container { c }, m_cellId { cellId }
{
}

auto cell_path::const_iterator::operator++() -> cell_path::const_iterator &
{
  m_cellId = m_container->Next(m_cellId);
  return *this;
}

auto cell_path::const_iterator::operator++(int) -> cell_path::const_iterator
{
  const_iterator copyOfThis { m_container, m_cellId };
  ++(*this);
  return copyOfThis;
}

auto cell_path::const_iterator::operator*() const -> const POINT_2I &
{
  return m_cellId;
}

auto cell_path::const_iterator::operator==(const cell_path::const_iterator &i) const -> bool
{
  return m_container == i.m_container && m_cellId == i.m_cellId;
}

cell_path::cell_path(POINT_2I begin, POINT_2I end) : m_begin { begin }, m_end { end }
{
}

auto cell_path::begin() const -> const_iterator
{
  return const_iterator(this, m_begin);
}

auto cell_path::end() const -> const_iterator
{
  return const_iterator(this, m_end);
}

auto cell_path::Next(POINT_2I cellId) const noexcept -> POINT_2I
{
  auto distanceToEnd = POINT_2I { m_end.x - cellId.x, m_end.y - cellId.y };
  auto absoluteDistanceToEnd = POINT_2I { std::abs(distanceToEnd.x), std::abs(distanceToEnd.y) };

  if( absoluteDistanceToEnd.x > absoluteDistanceToEnd.y )
  {
    auto positionOffset = distanceToEnd.x > 0 ? 1 : -1;
    return { cellId.x + positionOffset, cellId.y };
  }
  else
  {
    auto positionOffset = distanceToEnd.y > 0 ? 1 : -1;
    return { cellId.x, cellId.y + positionOffset};
  }
}
