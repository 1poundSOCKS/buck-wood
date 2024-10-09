#include "pch.h"
#include "render_object_collection.h"

auto render_object_collection::begin() const noexcept -> const_iterator
{
  return std::begin(m_layers);
}

auto render_object_collection::end() const noexcept -> const_iterator
{
  return std::end(m_layers);
}
