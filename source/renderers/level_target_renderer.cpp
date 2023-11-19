#include "pch.h"
#include "level_target_renderer.h"
#include "screen_render_brush_defs.h"

target_brush_selector::target_brush_selector(const target_brushes& brushes, const level_target& target) : m_brushes(brushes), m_target(target)
{
}

[[nodiscard]] auto target_brush_selector::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_brushes.Fill();
}

[[nodiscard]] auto target_brush_selector::Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_target.IsActivated() ? m_brushes.Activated() : m_brushes.NotActivated();
}

[[nodiscard]] auto target_brush_selector::StrokeWidth() const -> float
{
  return 8.0f;
}

target_brushes::target_brushes()
{
  const auto& renderTarget = render_target::renderTarget();
  m_notActivated = screen_render_brush_green.CreateBrush(renderTarget.get());
  m_activated =  screen_render_brush_red.CreateBrush(renderTarget.get());
  m_fill =  screen_render_brush_dark_grey.CreateBrush(renderTarget.get());
}

[[nodiscard]] auto target_brushes::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_fill;
}

[[nodiscard]] auto target_brushes::NotActivated() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_notActivated;
}

[[nodiscard]] auto target_brushes::Activated() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_activated;
}
