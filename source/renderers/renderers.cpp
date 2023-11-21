#include "pch.h"
#include "renderers.h"
#include "color_scale.h"
#include "slider_control.h"
#include "row_def.h"
#include "column_def.h"
#include "text_renderer.h"

renderer* renderer::m_instance = nullptr;

auto renderer::create() -> void
{
  destroy();
  m_instance = new renderer();
}

auto renderer::destroy() -> void
{
  if( m_instance )
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

renderer::renderer()
{
  m_starBrush = screen_render_brush_white.CreateBrush();
}

auto renderer::Render(const level_target& target) const -> void
{
  m_levelTargetRenderer.Write(target);
}

auto renderer::Render(const mine& levelMine) const -> void
{
  m_mineRenderer.Write(levelMine.Geometry());
}

auto renderer::Render(const level_asteroid& asteroid) const -> void
{
  m_solidObjectRenderer.Write(asteroid.Geometry());
}

auto renderer::Render(const blank_object& blankObject) const -> void
{
  m_blankRenderer.Write(blankObject.Geometry());
}

auto renderer::Render(const solid_object& solidObject) const -> void
{
  m_solidObjectRenderer.Write(solidObject.Geometry());
}

auto renderer::Render(const player_ship& playerShip) const -> void
{
  m_playerShipRenderer.Write(playerShip);
}

auto renderer::Render(const bullet& bulletInstance) const -> void
{
  m_bulletRenderer.Write(bulletInstance);
}

auto renderer::Render(const explosion_particle& particle) const -> void
{
  m_particleRenderer.Write(particle);
}

auto renderer::Render(const impact_particle& particle) const -> void
{
  m_particleRenderer.Write(particle);
}

auto renderer::Render(const level_star& star) const -> void
{
  static const auto rect = D2D1_RECT_F { -4, -4, 4, 4 };
  render_target::renderTarget()->FillRectangle(D2D1_RECT_F { rect.left + star.x, rect.top + star.y, rect.right + star.x, rect.bottom + star.y }, m_starBrush.get());
}

auto renderer::Render(const player_shields& playerShields) const -> void
{
  auto shieldRemaining = 100.0f - playerShields.GetDamagePercentage();
  slider_control damageSlider = { D2D1_RECT_F { 50, 500, 100, 800 } };
  render_target::renderTarget()->FillRectangle(damageSlider.GetSliderRect(shieldRemaining), m_playerShieldsBrushes.Fill().get());
  render_target::renderTarget()->DrawRectangle(damageSlider.GetBoundingRect(), m_playerShieldsBrushes.Draw().get(), m_playerShieldsBrushes.StrokeWidth());
}

auto renderer::Render(const menu_item& menuItem) const -> void
{
  m_menuRenderer.Write(menuItem);
}
