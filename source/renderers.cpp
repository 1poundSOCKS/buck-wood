#include "pch.h"
#include "renderers.h"
#include "color_scale.h"
#include "slider_control.h"

constexpr D2D1_RECT_F GetBulletRect()
{
  return { -5, -5, 5, 5 };
}

renderer* renderer::m_instance = nullptr;

auto renderer::create() -> void
{
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
  const auto& renderTarget = framework::renderTarget();
  m_playerExplosionBrush = screen_render_brush_white.CreateBrush(renderTarget.get());
  m_starBrush = screen_render_brush_white.CreateBrush(renderTarget.get());
}

auto renderer::Render(const level_target& target) const -> void
{
  Render(target.Geometry(), target_brush_selector { m_targetBrushes, target });
}

auto renderer::Render(const mine& levelMine) const -> void
{
  Render(levelMine.Geometry(), simple_brush_selector { m_mineBrushes } );
}

auto renderer::Render(const level_asteroid& asteroid) const -> void
{
  Render(asteroid.Geometry(), simple_brush_selector { m_asteroidBrushes });
}

auto renderer::Render(const player_ship& playerShip) const -> void
{
  if( playerShip.State() == player_ship::state::alive )
  {
    Render(playerShip.Geometry(), simple_brush_selector { m_playerShipBrushes });

    if( playerShip.ThrusterOn() )
    {
      std::vector<render_line> renderLines;
      auto renderLinesInserter = std::back_inserter(renderLines);
      std::vector<game_point> thrusterPoints;
      playerShip.GetTransformedThrusterGeometry(std::back_inserter(thrusterPoints));
      CreateDisconnectedRenderLines(thrusterPoints.cbegin(), thrusterPoints.cend(), renderLinesInserter, m_playerShipBrushes.Thruster().get(), 5);
      RenderLines(framework::renderTarget().get(), renderLines.cbegin(), renderLines.cend());
    }
  }
}

auto renderer::Render(const bullet& playerBullet) const -> void
{
  const D2D1_RECT_F rect = GetBulletRect();
  auto position = playerBullet.Position();
  const auto& brush = m_bulletBrushes[playerBullet.DistanceTravelled() / playerBullet.Range()];
  framework::renderTarget()->FillRectangle(D2D1_RECT_F { rect.left + position.x, rect.top + position.y, rect.right + position.x, rect.bottom + position.y }, brush.get());
}

auto renderer::Render(const explosion& playerExplosion) const -> void
{
  std::vector<render_point> renderParticles;

  std::transform(std::cbegin(playerExplosion.Particles()), std::cend(playerExplosion.Particles()), std::back_inserter(renderParticles), [this](auto particle) -> render_rect
  {
    const auto& brush = m_explosionBrushes[particle.DistanceTravelled() / particle.Range()];
    return particle.GetRenderRect(brush.get());
  });

  RenderPoints(framework::renderTarget().get(), renderParticles.cbegin(), renderParticles.cend());  
}

auto renderer::Render(const explosion_particle& particle) const -> void
{
  const auto& brush = m_explosionBrushes[particle.Age() / particle.Lifespan()];

  static const auto rect = D2D1_RECT_F { -4, -4, 4, 4 };
  const auto particleRect = D2D1_RECT_F { rect.left + particle.Position().x, rect.top + particle.Position().y, rect.right + particle.Position().x, rect.bottom + particle.Position().y };

  framework::renderTarget()->FillRectangle(particleRect, brush.get());
}

auto renderer::Render(const impact_particle& particle) const -> void
{
  const auto& brush = m_impactBrushes[particle.Age() / particle.Lifespan()];

  static const auto rect = D2D1_RECT_F { -4, -4, 4, 4 };
  const auto particleRect = D2D1_RECT_F { rect.left + particle.Position().x, rect.top + particle.Position().y, rect.right + particle.Position().x, rect.bottom + particle.Position().y };

  framework::renderTarget()->FillRectangle(particleRect, brush.get());
}

auto renderer::Render(const level_star& star) const -> void
{
  static const auto rect = D2D1_RECT_F { -4, -4, 4, 4 };
  framework::renderTarget()->FillRectangle(D2D1_RECT_F { rect.left + star.x, rect.top + star.y, rect.right + star.x, rect.bottom + star.y }, m_starBrush.get());
}

auto renderer::Render(const player_shields& playerShields) const -> void
{
  auto shieldRemaining = 100.0f - playerShields.GetDamagePercentage();

  slider_control damageSlider = { D2D1_RECT_F { 0, 0, 500, 100 } };

  framework::renderTarget()->FillRectangle(damageSlider.GetSliderRect(shieldRemaining), m_playerShieldsBrushes.Fill().get());
  framework::renderTarget()->DrawRectangle(damageSlider.GetBoundingRect(), m_playerShieldsBrushes.Draw().get(), m_playerShieldsBrushes.StrokeWidth());
}

template <typename brush_selector>
auto renderer::Render(const path_geometry& geometry, const brush_selector& brushSelector) const -> void
{
  framework::renderTarget()->FillGeometry(geometry.Get(), brushSelector.Fill().get());
  framework::renderTarget()->DrawGeometry(geometry.Get(), brushSelector.Draw().get(), brushSelector.StrokeWidth());
}

template <typename brush_selector>
auto renderer::Render(const transformed_path_geometry& geometry, const brush_selector& brushSelector) const -> void
{
  framework::renderTarget()->FillGeometry(geometry.Get(), brushSelector.Fill().get());
  framework::renderTarget()->DrawGeometry(geometry.Get(), brushSelector.Draw().get(), brushSelector.StrokeWidth());
}
