#pragma once

#include "render_brush_def.h"
#include "render_target.h"

template <typename brush_type>
class render_brush_collection
{

public:

  using brush_initializer = std::tuple<brush_type, render_brush_def>;

  render_brush_collection<brush_type>(const auto& brushes);

};

template <typename brush_type>
render_brush_collection<brush_type>::render_brush_collection(const auto& brushes)
{
  for( const auto& brushInitializer : brushes )
  {
    const auto &[brushIndex, brushDef] = brushInitializer;
    auto brush = brushDef.CreateBrush(render_target::get().get());
  }
}
