#pragma once

#include "framework.h"
#include "level_objects.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"

class portal_renderer
{

public:

  auto Write(const portal& object, ID2D1Geometry* geometry) const -> void
  {
    m_portalRenderer.Write(geometry);

    constexpr static cyclic_interval cyclicInterval { 1.0f };

    float scale = cyclicInterval.get(object.Age());
    auto transform = D2D1::Matrix3x2F::Scale({scale, scale}) * D2D1::Matrix3x2F::Translation({object.Position().x, object.Position().y});
    auto transformedGeometry = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), geometry, transform);
    m_portalRenderer.Write(transformedGeometry.get());
  }

private:

  geometry_renderer m_portalRenderer { screen_render_brush_dark_grey.CreateBrush(), 10 };

};
