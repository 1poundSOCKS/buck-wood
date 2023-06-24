#pragma once

#include "screen_render_brush_defs.h"
#include "level_target.h"
#include "level_asteroid.h"
#include "player_ship.h"
#include "bullet.h"
#include "explosion.h"
#include "level_star.h"
#include "mine.h"
#include "color_scale_brushes.h"
#include "explosion_particle.h"

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

class mine_brushes
{
public:

  mine_brushes();

  [[nodiscard]] auto Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto StrokeWidth() const -> float;

private:

  winrt::com_ptr<ID2D1SolidColorBrush> m_fill;
  winrt::com_ptr<ID2D1SolidColorBrush> m_draw;
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

  [[nodiscard]] auto Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;
  [[nodiscard]] auto StrokeWidth() const -> float;
  [[nodiscard]] auto Thruster() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;

private:

  winrt::com_ptr<ID2D1SolidColorBrush> m_fill;
  winrt::com_ptr<ID2D1SolidColorBrush> m_draw;
  winrt::com_ptr<ID2D1SolidColorBrush> m_thruster;
};

class bullet_brushes
{
public:

  bullet_brushes();
  [[nodiscard]] auto Fill(float fadeRatio) const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;

private:

  color_scale_brushes m_brushes;
};

class explosion_brushes
{
public:

  explosion_brushes();
  [[nodiscard]] auto Fill(float fadeRatio) const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;

private:

  color_scale_brushes m_brushes;
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

class bullet_brush_selector
{
public:

  bullet_brush_selector(const bullet_brushes& brushes, const bullet& playerBullet);
  [[nodiscard]] auto Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&;

private:
  const bullet_brushes& m_brushes;
  const bullet& m_bullet;

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
  static auto destroy() -> void;
  static auto render(const auto& object) -> void;
  static auto render_all(std::ranges::input_range auto&& objects) -> void;

private:

  static renderer* m_instance;
  renderer();

  auto Render(const level_target& target) const -> void;
  auto Render(const mine& mine) const -> void;
  auto Render(const level_asteroid& asteroid) const -> void;
  auto Render(const player_ship& playerShip) const -> void;
  auto Render(const bullet& playerBullet) const -> void;
  auto Render(const explosion& playerExplosion) const -> void;
  auto Render(const explosion_particle& particle) const -> void;
  auto Render(const level_star& star) const -> void;

  template <typename brush_selector> auto Render(const path_geometry& geometry, const brush_selector& object) const -> void;
  template <typename brush_selector> auto Render(const transformed_path_geometry& geometry, const brush_selector& brushSelector) const -> void;

  target_brushes m_targetBrushes;
  mine_brushes m_mineBrushes;
  asteroid_brushes m_asteroidBrushes;
  player_ship_brushes m_playerShipBrushes;
  bullet_brushes m_bulletBrushes;
  explosion_brushes m_explosionBrushes;
  winrt::com_ptr<ID2D1SolidColorBrush> m_playerExplosionBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_starBrush;
};

auto renderer::render(const auto& object) -> void
{
  m_instance->Render(object);
}

auto renderer::render_all(std::ranges::input_range auto&& objects) -> void
{
  for( const auto& object : objects )
  {
    m_instance->Render(object);
  }
}
