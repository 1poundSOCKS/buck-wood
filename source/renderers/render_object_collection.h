#pragma once

#include "render_order.h"
#include "render_layer.h"

class render_object_collection
{

public:

  using value_type = std::vector<render_layer>;
  using const_iterator = value_type::const_iterator;

  render_object_collection(std::ranges::input_range auto&& objects);

  [[nodiscard]] auto begin() const noexcept -> const_iterator;
  [[nodiscard]] auto end() const noexcept -> const_iterator;

private:

  value_type m_layers;

};

render_object_collection::render_object_collection(std::ranges::input_range auto&& objects)
{
  m_layers.reserve(render_order::end());

  auto objectCount = std::ranges::distance(objects);

  for( auto renderOrderIndex = render_order::begin(); renderOrderIndex < render_order::end(); ++renderOrderIndex )
  {
    m_layers.emplace_back(renderOrderIndex, objectCount);
  }

  for( const auto& object : objects )
  {
    auto renderOrderIndex = render_order::get(object);
    m_layers[renderOrderIndex].Add(object);
  }
}
