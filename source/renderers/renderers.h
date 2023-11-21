#pragma once

#include "level_asteroid.h"
#include "solid_objects.h"
#include "blank_objects.h"
#include "explosion.h"
#include "level_star.h"
#include "mine.h"
#include "explosion_particle.h"
#include "impact_particle.h"
#include "player_shields.h"

#include "level_target_renderer.h"
#include "player_ship_renderer.h"
#include "bullet_renderer.h"
#include "particle_renderer.h"
#include "menu_renderer.h"
#include "slider_control_renderer.h"

class renderer
{

public:

  static auto create() -> void;
  static auto destroy() -> void;
  static auto render(const auto& object) -> void;
  static auto render_all(std::ranges::input_range auto&& objects) -> void;

private:

  auto Render(const level_target& target) const -> void;
  auto Render(const mine& mine) const -> void;
  auto Render(const level_asteroid& asteroid) const -> void;
  auto Render(const blank_object& blankObject) const -> void;
  auto Render(const solid_object& solidObject) const -> void;
  auto Render(const player_ship& playerShip) const -> void;
  auto Render(const bullet& bulletInstance) const -> void;
  auto Render(const explosion_particle& particle) const -> void;
  auto Render(const impact_particle& particle) const -> void;
  auto Render(const level_star& star) const -> void;
  auto Render(const player_shields& playerShields) const -> void;
  auto Render(const menu_item& menuItem) const -> void;

private:

  static renderer* m_instance;
  level_target_renderer m_levelTargetRenderer;
  geometry_renderer m_mineRenderer { screen_render_brush_red.CreateBrush(), screen_render_brush_grey.CreateBrush(), 3 };
  player_ship_renderer m_playerShipRenderer;
  bullet_renderer m_bulletRenderer;
  geometry_renderer m_solidObjectRenderer { screen_render_brush_grey.CreateBrush(), screen_render_brush_dark_grey.CreateBrush(), 6 };
  geometry_renderer m_blankRenderer { screen_render_brush_black.CreateBrush() };
  particle_renderer m_particleRenderer;
  menu_renderer m_menuRenderer;
  slider_control_renderer m_playerShieldsRenderer;
  render_brush m_starBrush { screen_render_brush_white.CreateBrush() };

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

inline auto renderer::Render(const level_target& target) const -> void
{
  m_levelTargetRenderer.Write(target);
}

inline auto renderer::Render(const mine& levelMine) const -> void
{
  m_mineRenderer.Write(levelMine.Geometry());
}

inline auto renderer::Render(const level_asteroid& asteroid) const -> void
{
  m_solidObjectRenderer.Write(asteroid.Geometry());
}

inline auto renderer::Render(const blank_object& blankObject) const -> void
{
  m_blankRenderer.Write(blankObject.Geometry());
}

inline auto renderer::Render(const solid_object& solidObject) const -> void
{
  m_solidObjectRenderer.Write(solidObject.Geometry());
}

inline auto renderer::Render(const player_ship& playerShip) const -> void
{
  m_playerShipRenderer.Write(playerShip);
}

inline auto renderer::Render(const bullet& bulletInstance) const -> void
{
  m_bulletRenderer.Write(bulletInstance);
}

inline auto renderer::Render(const explosion_particle& particle) const -> void
{
  m_particleRenderer.Write(particle);
}

inline auto renderer::Render(const impact_particle& particle) const -> void
{
  m_particleRenderer.Write(particle);
}

inline auto renderer::Render(const level_star& star) const -> void
{
  static const D2D1_RECT_F rect { -4, -4, 4, 4 };
  D2D1_RECT_F renderRect { rect.left + star.x, rect.top + star.y, rect.right + star.x, rect.bottom + star.y };
  render_target::renderTarget()->FillRectangle(renderRect, m_starBrush.get());
}

inline auto renderer::Render(const player_shields& playerShields) const -> void
{
  m_playerShieldsRenderer.Write(playerShields);
}

inline auto renderer::Render(const menu_item& menuItem) const -> void
{
  m_menuRenderer.Write(menuItem);
}
