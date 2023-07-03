#include "pch.h"
#include "render_brushes.h"
#include "framework.h"

target_brushes::target_brushes()
{
  const auto& renderTarget = framework::renderTarget();
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

mine_brushes::mine_brushes()
{
  const auto& renderTarget = framework::renderTarget();
  m_fill =  screen_render_brush_red.CreateBrush(renderTarget.get());
  m_draw = screen_render_brush_grey.CreateBrush(renderTarget.get());
}

[[nodiscard]] auto mine_brushes::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_fill;
}

[[nodiscard]] auto mine_brushes::Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_draw;
}

[[nodiscard]] auto mine_brushes::StrokeWidth() const -> float
{
  return 3.0f;
}

asteroid_brushes::asteroid_brushes()
{
  const auto& renderTarget = framework::renderTarget();
  m_fill = screen_render_brush_grey.CreateBrush(renderTarget.get());
  m_draw = screen_render_brush_dark_grey.CreateBrush(renderTarget.get());
}

[[nodiscard]] auto asteroid_brushes::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_fill;
}

[[nodiscard]] auto asteroid_brushes::Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_draw;
}

[[nodiscard]] auto asteroid_brushes::StrokeWidth() const -> float
{
  return 6.0f;
}

player_ship_brushes::player_ship_brushes()
{
  const auto& renderTarget = framework::renderTarget();
  m_fill = screen_render_brush_dark_grey.CreateBrush(renderTarget.get());
  m_draw = screen_render_brush_white.CreateBrush(renderTarget.get());
  m_thruster = screen_render_brush_red.CreateBrush(renderTarget.get());
}

[[nodiscard]] auto player_ship_brushes::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_fill;
}

[[nodiscard]] auto player_ship_brushes::Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_draw;
}

[[nodiscard]] auto player_ship_brushes::StrokeWidth() const -> float
{
  return 2.0f;
}

[[nodiscard]] auto player_ship_brushes::Thruster() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_thruster;
}

bullet_brushes::bullet_brushes() : m_brushes { color_scale { D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), 10 } }
{
}

[[nodiscard]] auto bullet_brushes::Fill(float fadeRatio) const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_brushes[fadeRatio];
}

explosion_brushes::explosion_brushes() : m_brushes { color_scale { D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), 10 } }
{
}

[[nodiscard]] auto explosion_brushes::Fill(float fadeRatio) const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_brushes[fadeRatio];
}

impact_brushes::impact_brushes() : m_brushes { color_scale { D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f), D2D1::ColorF(0.5f, 0.0f, 0.0f, 1.0f), 10 } }
{
}

[[nodiscard]] auto impact_brushes::Fill(float fadeRatio) const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_brushes[fadeRatio];
}


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

bullet_brush_selector::bullet_brush_selector(const bullet_brushes& brushes, const bullet& playerBullet) : m_brushes { brushes }, m_bullet { playerBullet }
{
}

[[nodiscard]] auto bullet_brush_selector::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_brushes.Fill(m_bullet.DistanceTravelled() / m_bullet.Range());
}
