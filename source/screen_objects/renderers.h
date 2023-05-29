#pragma once

#include "render_brush_defs.h"
#include "level_target.h"
#include "level_asteroid.h"
#include "player_ship.h"

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

class asteroid_brushes
{
public:

  asteroid_brushes();

  [[nodiscard]] auto Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto StrokeWidth() const -> float;

private:

  winrt::com_ptr<ID2D1SolidColorBrush> m_fill;
  winrt::com_ptr<ID2D1SolidColorBrush> m_draw;
};

class player_ship_brushes
{
public:

  player_ship_brushes();

  [[nodiscard]] auto Ship() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto Thruster() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;

private:

  winrt::com_ptr<ID2D1SolidColorBrush> m_ship;
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

class renderer
{

public:

  static auto create() -> void;
  static auto render(const auto& object) -> void;

private:

  static renderer* m_instance;
  renderer();

  auto Render(const level_target& target) const -> void;
  auto Render(const level_asteroid& asteroid) const -> void;
  auto Render(const player_ship& playerShip) const -> void;

  template <typename brush_selector> auto Render(const path_geometry& geometry, const brush_selector& object) const -> void;

  target_brushes m_targetBrushes;
  asteroid_brushes m_asteroidBrushes;
  player_ship_brushes m_playerShipBrushes;
};

auto renderer::render(const auto& object) -> void
{
  m_instance->Render(object);
}
