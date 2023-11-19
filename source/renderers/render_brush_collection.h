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

private:

  collection_type m_brushes;

};

template <typename brush_type>
render_brush_collection<brush_type>::render_brush_collection(const auto& brushes)
{
  for( const auto& [brushIndex, brushDef] : brushes )
  {
    auto brush = brushDef.CreateBrush(render_target::renderTarget().get());

    auto requiredSize = static_cast<std::underlying_type<brush_type>::type>(brushIndex) + 1;

    if( m_brushes.size() < requiredSize )
    {
      m_brushes.resize(requiredSize);
    }

    m_brushes[brushIndex] = brush;
  }
}
