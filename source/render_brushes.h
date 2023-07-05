#pragma once

#include "screen_render_brush_defs.h"
#include "color_scale_brushes.h"
#include "level_target.h"
#include "bullet.h"

class filled_geometry_brushes
{
public:

  filled_geometry_brushes(D2D_COLOR_F fillColor, D2D_COLOR_F drawColor, float strokeWidth);
  filled_geometry_brushes(const render_brush_def& fillBrushDef, const render_brush_def& drawBrushDef, float strokeWidth);

  [[nodiscard]] auto Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto StrokeWidth() const -> float;

private:

  winrt::com_ptr<ID2D1SolidColorBrush> m_fill;
  winrt::com_ptr<ID2D1SolidColorBrush> m_draw;
  float m_strokeWidth { 0 };
};

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

class player_ship_brushes : public filled_geometry_brushes
{
public:

  player_ship_brushes();

  [[nodiscard]] auto Thruster() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;

private:

  winrt::com_ptr<ID2D1SolidColorBrush> m_thruster;
};

class target_brush_selector
{
public:

  target_brush_selector(const target_brushes& brushes, const level_target& target);

  [[nodiscard]] auto Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto StrokeWidth() const -> float;

private:
  const target_brushes& m_brushes;
  const level_target& m_target;

};

template <typename brushes_container>
class simple_brush_selector
{
public:

  simple_brush_selector(const brushes_container& brushes);

  [[nodiscard]] auto Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto StrokeWidth() const -> float;

private:
  const brushes_container& m_brushes;
};

template <typename brushes_container>
simple_brush_selector<brushes_container>::simple_brush_selector(const brushes_container& brushes) : m_brushes(brushes)
{
}

template <typename brushes_container>
[[nodiscard]] auto simple_brush_selector<brushes_container>::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_brushes.Fill();
}

template <typename brushes_container>
[[nodiscard]] auto simple_brush_selector<brushes_container>::Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_brushes.Draw();
}

template <typename brushes_container>
[[nodiscard]] auto simple_brush_selector<brushes_container>::StrokeWidth() const -> float
{
  return m_brushes.StrokeWidth();
}

inline filled_geometry_brushes::filled_geometry_brushes(D2D_COLOR_F fillColor, D2D_COLOR_F drawColor, float strokeWidth) : 
  m_strokeWidth { strokeWidth }
{
  const auto& renderTarget = framework::renderTarget();
  m_fill = render_brush_def { fillColor }.CreateBrush(renderTarget.get());
  m_draw = render_brush_def { drawColor }.CreateBrush(renderTarget.get());
}

inline filled_geometry_brushes::filled_geometry_brushes(const render_brush_def& fillBrushDef, const render_brush_def& drawBrushDef, float strokeWidth) : 
  m_strokeWidth { strokeWidth }
{
  const auto& renderTarget = framework::renderTarget();
  m_fill = fillBrushDef.CreateBrush(renderTarget.get());
  m_draw = drawBrushDef.CreateBrush(renderTarget.get());
}

[[nodiscard]] inline auto filled_geometry_brushes::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_fill;
}

[[nodiscard]] inline auto filled_geometry_brushes::Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_draw;
}

[[nodiscard]] inline auto filled_geometry_brushes::StrokeWidth() const -> float
{
  return m_strokeWidth;
}
