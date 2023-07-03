#pragma once

#include "render_brushes.h"
#include "level_asteroid.h"
#include "player_ship.h"
#include "explosion.h"
#include "level_star.h"
#include "mine.h"
#include "explosion_particle.h"
#include "impact_particle.h"
#include "player_shields.h"

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
  auto Render(const impact_particle& particle) const -> void;
  auto Render(const level_star& star) const -> void;

  auto Render(const player_shields& playerShields) const -> void;

  template <typename brush_selector> auto Render(const path_geometry& geometry, const brush_selector& object) const -> void;
  template <typename brush_selector> auto Render(const transformed_path_geometry& geometry, const brush_selector& brushSelector) const -> void;

  target_brushes m_targetBrushes;
  mine_brushes m_mineBrushes;
  asteroid_brushes m_asteroidBrushes;
  player_ship_brushes m_playerShipBrushes;
  bullet_brushes m_bulletBrushes;
  explosion_brushes m_explosionBrushes;
  impact_brushes m_impactBrushes;
  filled_geometry_brushes m_playerShieldsBrushes { D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), 3 };
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
