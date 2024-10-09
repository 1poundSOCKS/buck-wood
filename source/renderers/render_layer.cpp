#include "pch.h"
#include "render_layer.h"

render_layer::render_layer(render_order::value_type orderIndex, value_type::size_type initialCapacity) : m_orderIndex { orderIndex }
{
  m_objects.reserve(initialCapacity);
}

auto render_layer::Add(const default_object &object) -> void
{
  m_objects.emplace_back(&object);
}

auto render_layer::begin() const noexcept -> const_iterator
{
  return std::begin(m_objects);
}

auto render_layer::end() const noexcept -> const_iterator
{
  return std::end(m_objects);
}
