#pragma once

#include "dynamic_object.h"
#include "hud_target.h"
#include "target_position.h"
#include "player_destination.h"
#include "level_container.h"

#include "default_object_renderer.h"

#include "geometric_object_transform.h"
#include "level_object_geometry.h"
#include "transformed_level_object_geometry.h"

#include "particle_renderer.h"

#include "menu_renderer.h"
#include "slider_control_renderer.h"
#include "diagnostics_renderer.h"
#include "level_radar_renderer.h"

#include "game_score_renderer.h"
#include "level_title_renderer.h"

#include "line_to_target.h"

#include "render_order.h"

#include "energy_bar.h"

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

  auto Render(const default_object& object) const -> void;
  auto Render(const blank_object& blankObject) const -> void;
  auto Render(const particle& particle) const -> void;
  auto Render(const health_status& playerShields) const -> void;
  auto Render(const menu_item& menuItem) const -> void;
  auto Render(const level_radar& levelRadar, std::ranges::input_range auto&& objects) -> void;
  auto Render(const hud_target& hudTarget) const -> void;
  auto Render(const game_score& gameScore) const -> void;
  auto Render(const level_title& levelTitle) const -> void;
  auto Render(const line_to_target& lineToTarget) const -> void;
  auto Render(const energy_bar& energyBar) const -> void;
  auto Render(const level_container& levelContainer) const -> void;
  auto Write(const default_object& object) const -> void;
  auto Write(const portal &object) const -> void;
  auto Write(const auto& object) const -> void;

private:

  inline static renderer* m_instance { nullptr };

  menu_renderer m_menuRenderer;
  slider_control_renderer m_playerShieldsRenderer;
  diagnostics_renderer m_diagnosticsRenderer;
  level_radar_renderer m_levelRadarRenderer;
  geometry_renderer m_hudTargetRenderer { screen_render_brush_yellow.CreateBrush(), 5 };
  winrt::com_ptr<ID2D1SolidColorBrush> m_playerShieldsBrush { screen_render_brush_white.CreateBrush() };
  geometry_renderer m_playerShieldRenderer { screen_render_brush_grey.CreateBrush(), 5 };
  game_score_renderer m_gameScoreRenderer;
  level_title_renderer m_levelTitleRenderer;

  default_object_renderer m_defaultObjectRenderer;
  geometry_renderer m_boundaryRenderer { screen_render_brush_grey.CreateBrush(), 20 };
  particle_renderer m_particleRenderer;
  winrt::com_ptr<ID2D1SolidColorBrush> m_lineToTargetBrush { screen_render_brush_grey.CreateBrush() };
  winrt::com_ptr<ID2D1SolidColorBrush> m_energyBarBorderBrush { screen_render_brush_white.CreateBrush() };
  winrt::com_ptr<ID2D1SolidColorBrush> m_energyBarFillBrush { screen_render_brush_green.CreateBrush() };
  geometry_renderer m_floorCellRenderer { screen_render_brush_black.CreateBrush() };
  geometry_renderer m_exitCellRenderer { screen_render_brush_yellow.CreateBrush(), screen_render_brush_grey.CreateBrush(), 5 };
  geometry_renderer m_defaultGeometryRenderer { screen_render_brush_grey.CreateBrush(), 5 };
  portal_renderer m_portalRenderer;
  winrt::com_ptr<ID2D1PathGeometry> m_defaultGeometry { level_geometries::TargetGeometry() };

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

inline auto renderer::Render(const blank_object& blankObject) const -> void
{
  m_boundaryRenderer.Write(static_cast<ID2D1PathGeometry*>(blankObject));
}

inline auto renderer::Render(const particle& particle) const -> void
{
  m_particleRenderer.Write(particle);
}

inline auto renderer::Render(const health_status& playerShields) const -> void
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

inline auto renderer::Render(const hud_target& hudTarget) const -> void
{
  auto bounds = hudTarget.Bounds();
  D2D1_SIZE_F position = { (bounds.left + bounds.right) / 2, (bounds.bottom + bounds.top) / 2 };
  auto transform = D2D1::Matrix3x2F::Scale({bounds.right - bounds.left, bounds.bottom - bounds.top}) * D2D1::Matrix3x2F::Translation(position);

  for( const auto& geometry : level_geometries::HudTargetGeometries() )
  {
    auto transformedGeometry = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), geometry.get(), transform);
    m_hudTargetRenderer.Write(transformedGeometry.get());
  }
}

inline auto renderer::Render(const game_score& gameScore) const -> void
{
  m_gameScoreRenderer.Write(gameScore);
}

inline auto renderer::Render(const level_title& levelTitle) const -> void
{
  m_levelTitleRenderer.Write(levelTitle);
}

inline auto renderer::Render(const default_object &object) const -> void
{
  std::visit([this](const auto& levelObject)
  {
    Write(levelObject);
  }, object.Get());
}

inline auto renderer::Render(const line_to_target& lineToTarget) const -> void
{
  render_target::get()->DrawLine(lineToTarget.m_start, lineToTarget.m_end, m_lineToTargetBrush.get(), 10);
}

inline auto renderer::Render(const energy_bar& energyBar) const -> void
{
  auto fillRect = energyBar.position;
  fillRect.right = fillRect.left + ( fillRect.right - fillRect.left ) * energyBar.value;
  render_target::get()->FillRectangle(fillRect, m_energyBarFillBrush.get());
  render_target::get()->DrawRectangle(energyBar.position, m_energyBarBorderBrush.get(), 5);
}

inline auto renderer::Render(const level_container &levelContainer) const -> void
{
  levelContainer.EnumerateFloorCollisionObjects([this](const collision_object& object){
    auto geometry = object.Geometry().GetRaw();
    m_floorCellRenderer.Write(geometry);
  });

  levelContainer.EnumerateExitCollisionObjects([this](const collision_object& object){
    auto geometry = object.Geometry().GetRaw();
    m_exitCellRenderer.Write(geometry);
  });

  levelContainer.EnumerateNonInteractiveObjects([this](const auto& object)
  {
    Render(object);
  });

  levelContainer.EnumerateParticles([this](const auto& particle)
  {
    Render(particle);
  });

  for( int orderIndex = 0; orderIndex < render_order::max_value(); ++ orderIndex )
  {
    levelContainer.EnumerateInteractiveObjects([this,orderIndex](const auto& object)
    {
      if( render_order::get(object) == orderIndex )
      {
        Write(object);
      }
    });
  }
}

inline auto renderer::Write(const default_object &object) const -> void
{
  std::visit([this](const auto& object)
  {
    Write(object);
  }, object.Get());
}

inline auto renderer::Write(const portal &object) const -> void
{
  m_portalRenderer.Write(object);
}

inline auto renderer::Write(const auto &object) const -> void
{
  auto transformedGeometry = transformed_level_object_geometry { object };
  m_defaultObjectRenderer.Write(object, transformedGeometry.GetRaw());
}
