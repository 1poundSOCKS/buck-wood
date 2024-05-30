#pragma once

#include "framework.h"
#include "level_objects.h"
#include "geometry_renderer.h"
#include "screen_render_brush_defs.h"
#include "geometric_object_transform.h"
#include "geometry/level_geometries.h"

class portal_renderer
{

public:

  portal_renderer() : m_baseGeometry { level_geometries::get(object_type::portal) }
  {
  }

  auto Write(const portal& object) const -> void
  {
    auto transform = geometric_object_transform { object };
    auto transformedGeometry = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), m_baseGeometry.get(), transform.Get());
    Write(object, transformedGeometry.get());
  }

  auto Write(const portal& object, ID2D1Geometry* geometry) const -> void
  {
    const auto& renderer = object.CellId() ? m_exitPortalRenderer : m_entryPortalRenderer;

    renderer.Write(geometry);

    constexpr static cyclic_interval cyclicInterval { 1.0f };
    float scale = cyclicInterval.get(object.Age());
    auto transform = D2D1::Matrix3x2F::Scale({scale, scale}) * D2D1::Matrix3x2F::Translation({object.Position().x, object.Position().y});
    auto transformedGeometry = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), m_baseGeometry.get(), transform);

    renderer.Write(transformedGeometry.get());
  }

private:

  winrt::com_ptr<ID2D1Geometry> m_baseGeometry;
  geometry_renderer m_entryPortalRenderer { screen_render_brush_dark_grey.CreateBrush(), 10 };
  geometry_renderer m_exitPortalRenderer { screen_render_brush_green.CreateBrush(), 10 };

};
