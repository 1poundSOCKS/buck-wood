#pragma once

#include "level_target.h"
#include "geometry_renderer.h"

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

class level_target_renderer
{

public:

  auto Write(const level_target& levelTarget) const -> void;

private:

  geometry_renderer m_geometryRenderer;
  target_brushes m_targetBrushes;

};

inline auto level_target_renderer::Write(const level_target& levelTarget) const -> void
{
  levelTarget.IsActivated() ? m_geometryRenderer.Write(levelTarget.Geometry(), m_targetBrushes.Fill(), m_targetBrushes.Activated(), 5) :
    m_geometryRenderer.Write(levelTarget.Geometry(), m_targetBrushes.Fill(), m_targetBrushes.NotActivated(), 5);
}
