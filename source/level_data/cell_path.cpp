#include "pch.h"
#include "cell_path.h"

cell_path::const_iterator::const_iterator(const container& c, cell_id cellId) : m_container { c }, m_cellId { cellId }
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

cell_path::container::container(cell_id begin, cell_id end) : m_begin { begin }, m_end { end }
{
}

auto cell_path::container::begin() const -> const_iterator
{
  return const_iterator(*this, m_begin);
}

auto cell_path::container::end() const -> const_iterator
{
  return const_iterator(*this, m_end);
}

auto cell_path::container::Next(cell_id cellId) const noexcept -> cell_id
{
  return m_end;
}
