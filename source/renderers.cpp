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
  m_blankBrush = screen_render_brush_black.CreateBrush(renderTarget.get());
}

auto renderer::Render(const level_target& target) const -> void
{
  RenderWithBorder(target.Geometry(), target_brush_selector { m_targetBrushes, target });
}

auto renderer::Render(const mine& levelMine) const -> void
{
  RenderWithBorder(levelMine.Geometry(), simple_brush_selector { m_mineBrushes } );
}

auto renderer::Render(const level_asteroid& asteroid) const -> void
{
  RenderWithBorder(asteroid.Geometry(), simple_brush_selector { m_asteroidBrushes });
}

auto renderer::Render(const blank_object& object) const -> void
{
  RenderWithNoBorder(object.Geometry(), m_blankBrush.get());
}

auto renderer::Render(const solid_object& object) const -> void
{
  RenderWithBorder(object.Geometry(), simple_brush_selector { m_asteroidBrushes });
}

auto renderer::Render(const player_ship& playerShip) const -> void
{
  if( playerShip.State() == player_ship::state::alive )
  {
    RenderWithBorder(playerShip.Geometry(), simple_brush_selector { m_playerShipBrushes });

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

  slider_control damageSlider = { D2D1_RECT_F { 50, 500, 100, 800 } };

  framework::renderTarget()->FillRectangle(damageSlider.GetSliderRect(shieldRemaining), m_playerShieldsBrushes.Fill().get());
  framework::renderTarget()->DrawRectangle(damageSlider.GetBoundingRect(), m_playerShieldsBrushes.Draw().get(), m_playerShieldsBrushes.StrokeWidth());
}

auto renderer::Render(const button& buttonObject) const -> void
{
  const auto& rect = buttonObject.Rect();
  const auto& text = buttonObject.Text();
  
  if( buttonObject.HoverState() )
  {
    framework::renderTarget()->DrawRectangle(rect, m_menuBrushes.Get(menu_brushes::id::border).get(), 1);

    RenderText(framework::renderTarget().get(), m_menuBrushes.Get(menu_brushes::id::button_hover).get(), 
      m_renderText.get(render_text::selector::menu_text_hover).get(), text.c_str(), rect, 
      DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
  }
  else
  {
    framework::renderTarget()->DrawRectangle(rect, m_menuBrushes.Get(menu_brushes::id::border).get(), 1);

    RenderText(framework::renderTarget().get(), m_menuBrushes.Get(menu_brushes::id::button).get(), 
      m_renderText.get(render_text::selector::menu_text_default).get(), text.c_str(), rect, 
      DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER);
  }
}

auto renderer::Render(const setting_slider& settingSlider) const -> void
{
  const auto& rect = settingSlider.Rect();

  framework::renderTarget()->DrawRectangle(rect, m_menuBrushes.Get(menu_brushes::id::border).get(), 1);
}

auto renderer::Render(const menu_item& menuItem) const -> void
{
  struct render_menu_item_visitor
  {
      void operator()(const button& item)
      {
        renderer::render(item);
      }

      void operator()(const setting_slider& item)
      {
        renderer::render(item);
      }
  };

  std::visit(render_menu_item_visitor {}, menuItem.Get());
}

template <typename brush_selector>
auto renderer::RenderWithBorder(const path_geometry& geometry, const brush_selector& brushSelector) const -> void
{
  framework::renderTarget()->FillGeometry(geometry.Get(), brushSelector.Fill().get());
  framework::renderTarget()->DrawGeometry(geometry.Get(), brushSelector.Draw().get(), brushSelector.StrokeWidth());
}

template <typename brush_selector>
auto renderer::RenderWithBorder(const transformed_path_geometry& geometry, const brush_selector& brushSelector) const -> void
{
  framework::renderTarget()->FillGeometry(geometry.Get(), brushSelector.Fill().get());
  framework::renderTarget()->DrawGeometry(geometry.Get(), brushSelector.Draw().get(), brushSelector.StrokeWidth());
}

auto renderer::RenderWithNoBorder(const path_geometry& geometry, ID2D1SolidColorBrush* brush) const -> void
{
  framework::renderTarget()->FillGeometry(geometry.Get(), brush);
}

auto renderer::RenderWithNoBorder(const transformed_path_geometry& geometry, ID2D1SolidColorBrush* brush) const -> void
{
  framework::renderTarget()->FillGeometry(geometry.Get(), brush);
}
