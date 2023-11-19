#pragma once

#include "level_target.h"
#include "geometry_renderer.h"
#include "render_brush_collection.h"
#include "screen_render_brush_defs.h"

class target_brushes
{
public:

  target_brushes();

  [[nodiscard]] auto Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto NotActivated() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto Activated() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;

private:

  winrt::com_ptr<ID2D1SolidColorBrush> m_fill;
  winrt::com_ptr<ID2D1SolidColorBrush> m_notActivated;
  winrt::com_ptr<ID2D1SolidColorBrush> m_activated;

};

class level_target_renderer
{

public:

  auto Write(const level_target& levelTarget) const -> void;

private:

  geometry_renderer m_geometryRenderer;
  target_brushes m_targetBrushes;

  enum class brushes { fill=0, activated,  not_activated };

  render_brush_collection<brushes> m_brushes
  {
    std::initializer_list<std::tuple<brushes, render_brush_def>>
    {
      std::tuple<brushes, render_brush_def> { brushes::fill, screen_render_brush_dark_grey }, 
      std::tuple<brushes, render_brush_def> { brushes::activated, screen_render_brush_red },
      std::tuple<brushes, render_brush_def> { brushes::not_activated, screen_render_brush_green }
  }};

};

inline auto level_target_renderer::Write(const level_target& levelTarget) const -> void
{
  levelTarget.IsActivated() ? m_geometryRenderer.Write(levelTarget.Geometry(), m_targetBrushes.Fill(), m_targetBrushes.Activated(), 5) :
    m_geometryRenderer.Write(levelTarget.Geometry(), m_targetBrushes.Fill(), m_targetBrushes.NotActivated(), 5);
}
