#pragma once

#include "render_text.h"
#include "render_brushes.h"
#include "level_asteroid.h"
#include "solid_objects.h"
#include "blank_objects.h"
#include "player_ship.h"
#include "explosion.h"
#include "level_star.h"
#include "mine.h"
#include "explosion_particle.h"
#include "impact_particle.h"
#include "player_shields.h"
#include "menu_item.h"

#include "level_target_renderer.h"
#include "player_ship_renderer.h"
#include "bullet_renderer.h"

class renderer
{

public:

  using VIEW_RECT = D2D1_RECT_F;

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
  auto Render(const blank_object& blankObject) const -> void;
  auto Render(const solid_object& solidObject) const -> void;
  auto Render(const player_ship& playerShip) const -> void;
  auto Render(const bullet& bulletInstance) const -> void;
  auto Render(const explosion_particle& particle) const -> void;
  auto Render(const impact_particle& particle) const -> void;
  auto Render(const level_star& star) const -> void;
  auto Render(const player_shields& playerShields) const -> void;
  auto Render(const menu_item& menuItem) const -> void;
  auto Render(const button& buttonObject) const -> void;
  auto Render(const setting_slider& settingSlider) const -> void;

  render_text m_renderText;
  menu_brushes m_menuBrushes;
  
  color_scale_brushes m_explosionBrushes { color_scale { D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), 10 } };
  color_scale_brushes m_impactBrushes { color_scale { D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f), D2D1::ColorF(0.5f, 0.0f, 0.0f, 1.0f), 10 } };
  
  filled_geometry_brushes m_playerShieldsBrushes { D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f), D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), 3 };
  
  render_brush m_playerExplosionBrush;
  render_brush m_starBrush;

  level_target_renderer m_levelTargetRenderer;
  geometry_renderer m_mineRenderer { screen_render_brush_red.CreateBrush(), screen_render_brush_grey.CreateBrush(), 3 };
  player_ship_renderer m_playerShipRenderer;
  bullet_renderer m_bulletRenderer;
  geometry_renderer m_solidObjectRenderer { screen_render_brush_grey.CreateBrush(), screen_render_brush_dark_grey.CreateBrush(), 6 };
  geometry_renderer m_blankRenderer { screen_render_brush_black.CreateBrush() };
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
