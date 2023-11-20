#pragma once

#include "render_brush_def.h"
#include "render_target.h"

template <typename brush_type>
class render_brush_collection
{

public:

  using initializer_type = std::tuple<brush_type, render_brush_def>;
  using collection_type = std::vector<render_brush>;

  render_brush_collection<brush_type>(const auto& brushes);
  [[nodiscard]] auto operator[](brush_type brushType) const -> const render_brush&;

private:

  collection_type m_brushes;

};

template <typename brush_type>
render_brush_collection<brush_type>::render_brush_collection(const auto& brushes)
{
  for( const auto& [brushType, brushDef] : brushes )
  {
    auto brushIndex = static_cast<std::underlying_type<brush_type>::type>(brushType);
    auto brush = brushDef.CreateBrush(render_target::renderTarget().get());

    auto requiredSize = brushIndex + 1;

    if( m_brushes.size() < requiredSize )
    {
      m_brushes.resize(requiredSize);
    }

    m_brushes[brushIndex] = brush;
  }
}

template <typename brush_type>
[[nodiscard]] auto render_brush_collection<brush_type>::operator[](brush_type brushType) const -> const render_brush&
{
  auto brushIndex = static_cast<std::underlying_type<brush_type>::type>(brushType);
  return m_brushes[brushIndex];
}
