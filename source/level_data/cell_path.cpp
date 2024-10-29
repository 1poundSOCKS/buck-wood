#include "pch.h"
#include "cell_path.h"

auto cell_path_iterator::operator++() -> cell_path_iterator &
{
  m_cellId = m_cellPath.Next(m_cellId);
  return *this;
}

auto cell_path_iterator::operator++(int) -> cell_path_iterator
{
  cell_path_iterator thisCopy { m_cellPath, m_cellId };
  m_cellId = m_cellPath.Next(m_cellId);
  return thisCopy;
}

auto cell_path_iterator::operator*() const -> const cell_id &
{
  return m_cellId;
}

auto cell_path_iterator::operator==(const cell_path_iterator &i) const -> bool
{
  return m_cellId == i.m_cellId;
}

cell_path_iterator::cell_path_iterator(const cell_path& cellPath, cell_id cellId) : m_cellPath { cellPath }, m_cellId{cellId}
{
}

cell_path::cell_path(cell_id begin, cell_id end) : m_begin { begin }, m_end { end }
{
}

auto cell_path::begin() const -> cell_path_iterator
{
  return cell_path_iterator(*this, m_begin);
}

auto cell_path::end() const -> cell_path_iterator
{
  return cell_path_iterator(*this, m_end);
}

auto cell_path::Next(cell_id cellId) const noexcept -> cell_id
{
  return m_end;
}
