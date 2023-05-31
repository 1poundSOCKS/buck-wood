#include "pch.h"
#include "renderers.h"

constexpr D2D1_RECT_F GetBulletRect()
{
  return { -5, -5, 5, 5 };
}

target_brushes::target_brushes()
{
  const auto& renderTarget = framework::renderTarget();
  m_notActivated = screen_render_brush_green.CreateBrush(renderTarget.get());
  m_activated =  screen_render_brush_red.CreateBrush(renderTarget.get());
  m_fill =  screen_render_brush_dark_grey.CreateBrush(renderTarget.get());
}

[[nodiscard]] auto target_brushes::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_fill;
}

[[nodiscard]] auto target_brushes::NotActivated() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_notActivated;
}

[[nodiscard]] auto target_brushes::Activated() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_activated;
}

asteroid_brushes::asteroid_brushes()
{
  const auto& renderTarget = framework::renderTarget();
  m_fill = screen_render_brush_grey.CreateBrush(renderTarget.get());
  m_draw = screen_render_brush_dark_grey.CreateBrush(renderTarget.get());
}

[[nodiscard]] auto asteroid_brushes::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_fill;
}

[[nodiscard]] auto asteroid_brushes::Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_draw;
}

[[nodiscard]] auto asteroid_brushes::StrokeWidth() const -> float
{
  return 6.0f;
}

player_ship_brushes::player_ship_brushes()
{
  const auto& renderTarget = framework::renderTarget();
  m_fill = screen_render_brush_dark_grey.CreateBrush(renderTarget.get());
  m_draw = screen_render_brush_white.CreateBrush(renderTarget.get());
  m_thruster = screen_render_brush_red.CreateBrush(renderTarget.get());
}

[[nodiscard]] auto player_ship_brushes::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_fill;
}

[[nodiscard]] auto player_ship_brushes::Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_draw;
}

[[nodiscard]] auto player_ship_brushes::StrokeWidth() const -> float
{
  return 2.0f;
}

[[nodiscard]] auto player_ship_brushes::Thruster() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_thruster;
}

target_brush_selector::target_brush_selector(const target_brushes& brushes, const level_target& target) : m_brushes(brushes), m_target(target)
{
}

[[nodiscard]] auto target_brush_selector::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_brushes.Fill();
}

[[nodiscard]] auto target_brush_selector::Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_target.IsActivated() ? m_brushes.Activated() : m_brushes.NotActivated();
}

[[nodiscard]] auto target_brush_selector::StrokeWidth() const -> float
{
  return 8.0f;
}

renderer* renderer::m_instance = nullptr;

auto renderer::create() -> void
{
  m_instance = new renderer();
}

renderer::renderer()
{
  const auto& renderTarget = framework::renderTarget();
  m_playerBulletBrush = screen_render_brush_yellow.CreateBrush(renderTarget.get());
  m_playerExplosionBrush = screen_render_brush_white.CreateBrush(renderTarget.get());
}

auto renderer::Render(const level_target& target) const -> void
{
  Render(target.Geometry(), target_brush_selector { m_targetBrushes, target });
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
  framework::renderTarget()->FillRectangle(D2D1_RECT_F { rect.left + position.x, rect.top + position.y, rect.right + position.x, rect.bottom + position.y }, m_playerBulletBrush.get());
}

auto renderer::Render(const explosion& playerExplosion) const -> void
{
  std::vector<render_point> renderParticles;

  std::transform(std::cbegin(playerExplosion.Particles()), std::cend(playerExplosion.Particles()), std::back_inserter(renderParticles), [this](auto particle) -> render_rect
  {
    return particle.GetRenderRect(this->m_playerExplosionBrush.get());
  });

  RenderPoints(framework::renderTarget().get(), renderParticles.cbegin(), renderParticles.cend());  
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
