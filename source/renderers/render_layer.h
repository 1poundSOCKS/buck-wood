#pragma once

#include "default_object.h"
#include "render_order.h"

class render_layer
{

public:

  using value_type = std::vector<const default_object*>;
  using const_iterator = value_type::const_iterator;

  render_layer(render_order::value_type orderIndex, value_type::size_type initialCapacity);

  auto Add(const default_object& object) -> void;

  [[nodiscard]] auto begin() const noexcept -> const_iterator;
  [[nodiscard]] auto end() const noexcept -> const_iterator;

private:

render_order::value_type m_orderIndex;
value_type m_objects;

};
