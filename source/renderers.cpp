#include "pch.h"
#include "renderers.h"
#include "color_scale.h"

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

mine_brushes::mine_brushes()
{
  const auto& renderTarget = framework::renderTarget();
  m_fill =  screen_render_brush_red.CreateBrush(renderTarget.get());
  m_draw = screen_render_brush_grey.CreateBrush(renderTarget.get());
}

[[nodiscard]] auto mine_brushes::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_fill;
}

[[nodiscard]] auto mine_brushes::Draw() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_draw;
}

[[nodiscard]] auto mine_brushes::StrokeWidth() const -> float
{
  return 3.0f;
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

bullet_brushes::bullet_brushes() : m_brushes { color_scale { D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), 10 } }
{
}

[[nodiscard]] auto bullet_brushes::Fill(float fadeRatio) const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_brushes[fadeRatio];
}

explosion_brushes::explosion_brushes() : m_brushes { color_scale { D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), 10 } }
{
}

[[nodiscard]] auto explosion_brushes::Fill(float fadeRatio) const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_brushes[fadeRatio];
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

bullet_brush_selector::bullet_brush_selector(const bullet_brushes& brushes, const bullet& playerBullet) : m_brushes { brushes }, m_bullet { playerBullet }
{
}

[[nodiscard]] auto bullet_brush_selector::Fill() const -> const winrt::com_ptr<ID2D1SolidColorBrush>&
{
  return m_brushes.Fill(m_bullet.DistanceTravelled() / m_bullet.Range());
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
  bullet_brush_selector brushSelector { m_bulletBrushes, playerBullet };
  auto brush = brushSelector.Fill();
  framework::renderTarget()->FillRectangle(D2D1_RECT_F { rect.left + position.x, rect.top + position.y, rect.right + position.x, rect.bottom + position.y }, brush.get());
}

auto renderer::Render(const explosion& playerExplosion) const -> void
{
  std::vector<render_point> renderParticles;

  std::transform(std::cbegin(playerExplosion.Particles()), std::cend(playerExplosion.Particles()), std::back_inserter(renderParticles), [this](auto particle) -> render_rect
  {
    auto brush = m_explosionBrushes.Fill(particle.DistanceTravelled() / particle.Range());
    return particle.GetRenderRect(brush.get());
  });

  RenderPoints(framework::renderTarget().get(), renderParticles.cbegin(), renderParticles.cend());  
}

auto renderer::Render(const explosion_particle& particle) const -> void
{
  auto brush = m_explosionBrushes.Fill(particle.Age() / particle.Lifespan());

  static const auto rect = D2D1_RECT_F { -4, -4, 4, 4 };
  const auto particleRect = D2D1_RECT_F { rect.left + particle.Position().x, rect.top + particle.Position().y, rect.right + particle.Position().x, rect.bottom + particle.Position().y };

  framework::renderTarget()->FillRectangle(particleRect, brush.get());
}

auto renderer::Render(const level_star& star) const -> void
{
  static const auto rect = D2D1_RECT_F { -4, -4, 4, 4 };
  framework::renderTarget()->FillRectangle(D2D1_RECT_F { rect.left + star.x, rect.top + star.y, rect.right + star.x, rect.bottom + star.y }, m_starBrush.get());
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
