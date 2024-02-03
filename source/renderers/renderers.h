#pragma once

#include "dynamic_object.h"
#include "level_objects.h"
#include "player_shields.h"
#include "hud_target.h"
#include "target_position.h"
#include "player_destination.h"

#include "level_target_renderer.h"
#include "bullet_renderer.h"
#include "particle_renderer.h"
#include "menu_renderer.h"
#include "slider_control_renderer.h"
#include "diagnostics_renderer.h"
#include "level_radar_renderer.h"
#include "level_geometries.h"

class renderer
{

public:

  static auto create() -> void;
  static auto destroy() -> void;
  static auto render(const auto& object) -> void;
  static auto render(const auto& object, std::ranges::input_range auto&& objects) -> void;
  static auto render_all(std::ranges::input_range auto&& objects) -> void;
  static auto renderDiagnostics() -> void;

private:

  auto Render(const dynamic_object<level_target>& targetObject) const -> void;
  auto Render(const dynamic_object<mine>& mineObject) const -> void;
  auto Render(const dynamic_object<duct_fan>& ductFan) const -> void;
  auto Render(const dynamic_object<level_asteroid>& asteroid) const -> void;
  auto Render(const blank_object& blankObject) const -> void;
  auto Render(const dynamic_object<player_ship>& playerShip) const -> void;
  auto Render(const bullet& bulletInstance) const -> void;
  auto Render(const explosion_particle& particle) const -> void;
  auto Render(const impact_particle& particle) const -> void;
  auto Render(const player_shields& playerShields) const -> void;
  auto Render(const menu_item& menuItem) const -> void;
  auto Render(const level_radar& levelRadar, std::ranges::input_range auto&& objects) -> void;
  auto Render(const thrust_particle& particle) const -> void;
  auto Render(const hud_target& hudTarget) const -> void;
  auto Render(const player_destination& playerDestination) const -> void;
  auto Render(const target_position& targetPosition) const -> void;
  auto RenderTarget(D2D1_POINT_2F position, float size, ID2D1SolidColorBrush* brush) const -> void;
  

private:

  inline static renderer* m_instance { nullptr };

  level_target_renderer m_levelTargetRenderer;
  geometry_renderer m_mineRenderer { screen_render_brush_red.CreateBrush(), 6 };
  geometry_renderer m_playerShipRenderer { screen_render_brush_white.CreateBrush(), 4 };
  bullet_renderer m_bulletRenderer;
  geometry_renderer m_asteroidRenderer { screen_render_brush_cyan.CreateBrush(), 8 };
  geometry_renderer m_blankRenderer { screen_render_brush_grey.CreateBrush(), 10 };
  particle_renderer m_particleRenderer { color_scale { D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), 10 } };
  particle_renderer m_thrustParticleRenderer { color_scale { D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f), D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), 10 } };
  menu_renderer m_menuRenderer;
  slider_control_renderer m_playerShieldsRenderer;
  diagnostics_renderer m_diagnosticsRenderer;
  level_radar_renderer m_levelRadarRenderer;
  geometry_renderer m_ductFanRenderer { screen_render_brush_white.CreateBrush(), 10 };
  geometry_renderer m_hudTargetRenderer { screen_render_brush_yellow.CreateBrush(), 5 };
  render_brush m_targetPositionBrush { screen_render_brush_red.CreateBrush() };
  render_brush m_playerDestinationBrush { screen_render_brush_green.CreateBrush() };

};

auto renderer::render(const auto& object) -> void
{
  m_instance->Render(object);
}

auto renderer::render(const auto& object, std::ranges::input_range auto&& objects) -> void
{
  m_instance->Render(object, objects);
}

auto renderer::render_all(std::ranges::input_range auto&& objects) -> void
{
  for( const auto& object : objects )
  {
    m_instance->Render(object);
  }
}

inline auto renderer::renderDiagnostics() -> void
{
  m_instance->m_diagnosticsRenderer.Write();
}

inline auto renderer::Render(const dynamic_object<level_target>& targetObject) const -> void
{
  m_levelTargetRenderer.Write(targetObject);
}

inline auto renderer::Render(const dynamic_object<mine>& mineObject) const -> void
{
  m_mineRenderer.Write(mineObject.Geometry());
}

inline auto renderer::Render(const dynamic_object<duct_fan>& ductFan) const -> void
{
  m_ductFanRenderer.Write(ductFan.Geometry());
}

inline auto renderer::Render(const dynamic_object<level_asteroid>& asteroid) const -> void
{
  m_asteroidRenderer.Write(asteroid.Geometry());
}

inline auto renderer::Render(const blank_object& blankObject) const -> void
{
  m_blankRenderer.Write(static_cast<ID2D1PathGeometry*>(blankObject));
}

inline auto renderer::Render(const dynamic_object<player_ship>& playerShip) const -> void
{
  m_playerShipRenderer.Write(playerShip.Geometry());
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

inline auto renderer::Render(const player_shields& playerShields) const -> void
{
  m_playerShieldsRenderer.Write(playerShields);
}

inline auto renderer::Render(const menu_item& menuItem) const -> void
{
  m_menuRenderer.Write(menuItem);
}

inline auto renderer::Render(const level_radar& levelRadar, std::ranges::input_range auto&& objects) -> void
{
  m_levelRadarRenderer.Write(levelRadar, objects);
}

inline auto renderer::Render(const thrust_particle& particle) const -> void
{
  m_thrustParticleRenderer.Write(particle);
}

inline auto renderer::Render(const hud_target& hudTarget) const -> void
{
  for( const auto& geometry : level_geometries::HudTargetGeometries() )
  {
    auto bounds = hudTarget.Bounds();
    D2D1_SIZE_F position = { (bounds.left + bounds.right) / 2, (bounds.bottom + bounds.top) / 2 };
    auto transform = D2D1::Matrix3x2F::Scale({bounds.right - bounds.left, bounds.bottom - bounds.top}) * D2D1::Matrix3x2F::Translation(position);
    auto transformedGeometry = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), geometry.get(), transform);
    m_hudTargetRenderer.Write(transformedGeometry.get());
  }
}

inline auto renderer::Render(const target_position& targetPosition) const -> void
{
  RenderTarget(targetPosition.Position(), 100, m_targetPositionBrush.get());
}

inline auto renderer::Render(const player_destination& playerDestination) const -> void
{
  RenderTarget(playerDestination.Position(), 50, m_playerDestinationBrush.get());
}

inline auto renderer::RenderTarget(D2D1_POINT_2F position, float size, ID2D1SolidColorBrush* brush) const -> void
{
  auto rect = direct2d::GetRectAtPosition(position, size, size);

  auto lineLength = size / 5;

  render_target::get()->DrawLine({ rect.left, rect.top + lineLength }, { rect.left, rect.top }, brush, 4.0f);
  render_target::get()->DrawLine({ rect.left, rect.top }, { rect.left + lineLength, rect.top }, brush, 4.0f);

  render_target::get()->DrawLine({ rect.right - lineLength, rect.top }, { rect.right, rect.top }, brush, 4.0f);
  render_target::get()->DrawLine({ rect.right, rect.top }, { rect.right, rect.top + lineLength }, brush, 4.0f);

  render_target::get()->DrawLine({ rect.right, rect.bottom - lineLength }, { rect.right, rect.bottom }, brush, 4.0f);
  render_target::get()->DrawLine({ rect.right, rect.bottom }, { rect.right - lineLength, rect.bottom }, brush, 4.0f);

  render_target::get()->DrawLine({ rect.left + lineLength, rect.bottom }, { rect.left, rect.bottom }, brush, 4.0f);
  render_target::get()->DrawLine({ rect.left, rect.bottom }, { rect.left, rect.bottom - lineLength }, brush, 4.0f);
}
