#include "pch.h"
#include "renderers.h"
#include "color_scale.h"
#include "slider_control.h"
#include "row_def.h"
#include "column_def.h"
#include "text_renderer.h"

struct render_point
{
  D2D1_RECT_F rect;
  ID2D1SolidColorBrush* brush;
};

using render_rect = render_point;

struct render_line
{
  render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, ID2D1SolidColorBrush* brush, float width = 1);
  
  D2D1_POINT_2F start, end;
  ID2D1SolidColorBrush* brush = nullptr;
  float width = 1;
};

render_line::render_line(const D2D1_POINT_2F& start, const D2D1_POINT_2F& end, ID2D1SolidColorBrush* brush, float width)
: start(start), end(end), brush(brush), width(width)
{
}

template <typename input_iterator_type>
void RenderPoints(
  ID2D1RenderTarget* renderTarget, 
  const typename input_iterator_type begin, 
  const typename input_iterator_type end)
{
  for( auto point = begin; point != end; ++point )
  {
    renderTarget->FillRectangle( point->rect, point->brush);
  }
}

template <typename input_iterator_type, typename insert_iterator_type>
void CreateDisconnectedRenderLines(
  typename input_iterator_type begin, 
  typename input_iterator_type end, 
  insert_iterator_type insertIterator, 
  ID2D1SolidColorBrush* brush, 
  float width, 
  float x=0, 
  float y=0)
{
  for( auto i = begin; i != end; std::advance(i, 2) )
  {
    auto next = std::next(i);
    if( next != end ) insertIterator = render_line({i->x + x, i->y + y}, {next->x + x, next->y + y}, brush, width);
  }
};

template <typename input_iterator_type>
void RenderLines(
  ID2D1RenderTarget* renderTarget, 
  input_iterator_type begin, 
  input_iterator_type end)
{
  for( auto line = begin; line != end; ++line )
  {
    renderTarget->DrawLine(line->start, line->end, line->brush, line->width);
  }
}

consteval std::array<D2D1_POINT_2F, 8> GetCursorRenderData()
{
  const float cursorSize = 20.0f;
  const float cursorSizeGap = 10.0f;

  return std::array<D2D1_POINT_2F, 8> {
    D2D1_POINT_2F { 0, -cursorSize },
    D2D1_POINT_2F { 0,-cursorSizeGap },
    D2D1_POINT_2F { 0,cursorSize },
    D2D1_POINT_2F { 0,cursorSizeGap },
    D2D1_POINT_2F { -cursorSize,0 },
    D2D1_POINT_2F { -cursorSizeGap,0 },
    D2D1_POINT_2F { cursorSize,0 },
    D2D1_POINT_2F { cursorSizeGap,0 }
  };
}

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
  const auto& renderTarget = render_target::renderTarget();
  m_mouseCursorBrush = screen_render_brush_white.CreateBrush(renderTarget.get());
  m_playerExplosionBrush = screen_render_brush_white.CreateBrush(renderTarget.get());
  m_starBrush = screen_render_brush_white.CreateBrush(renderTarget.get());
  m_blankBrush = screen_render_brush_black.CreateBrush(renderTarget.get());
}

auto renderer::RenderMouseCursor(float x, float y) const -> void
{
  static const float cursorSize = 20.0f;
  static const float cursorSizeGap = 10.0f;

  auto mouseCursorRenderData = GetCursorRenderData();

  std::vector<render_line> renderLines;

  CreateDisconnectedRenderLines(
    mouseCursorRenderData.cbegin(), 
    mouseCursorRenderData.cend(), 
    std::back_inserter(renderLines), 
    m_starBrush.get(), 5, x, y);
  
  RenderLines(render_target::renderTarget().get(), renderLines.cbegin(), renderLines.cend());
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
  m_playerShipRenderer.Write(playerShip);
}

auto renderer::Render(const bullet& playerBullet) const -> void
{
  const D2D1_RECT_F rect = GetBulletRect();
  auto position = playerBullet.Position();
  const auto& brush = m_bulletBrushes[playerBullet.DistanceTravelled() / playerBullet.Range()];
  render_target::renderTarget()->FillRectangle(D2D1_RECT_F { rect.left + position.x, rect.top + position.y, rect.right + position.x, rect.bottom + position.y }, brush.get());
}

auto renderer::Render(const explosion& playerExplosion) const -> void
{
  std::vector<render_point> renderParticles;

  std::transform(std::cbegin(playerExplosion.Particles()), std::cend(playerExplosion.Particles()), std::back_inserter(renderParticles), [this](auto particle) -> render_rect
  {
    const auto& brush = m_explosionBrushes[particle.DistanceTravelled() / particle.Range()];
    auto rect = particle.GetRenderRect();
    return { rect, brush.get() };
  });

  RenderPoints(render_target::renderTarget().get(), renderParticles.cbegin(), renderParticles.cend());  
}

auto renderer::Render(const explosion_particle& particle) const -> void
{
  const auto& brush = m_explosionBrushes[particle.Age() / particle.Lifespan()];

  static const auto rect = D2D1_RECT_F { -4, -4, 4, 4 };
  const auto particleRect = D2D1_RECT_F { rect.left + particle.Position().x, rect.top + particle.Position().y, rect.right + particle.Position().x, rect.bottom + particle.Position().y };

  render_target::renderTarget()->FillRectangle(particleRect, brush.get());
}

auto renderer::Render(const impact_particle& particle) const -> void
{
  const auto& brush = m_impactBrushes[particle.Age() / particle.Lifespan()];

  static const auto rect = D2D1_RECT_F { -4, -4, 4, 4 };
  const auto particleRect = D2D1_RECT_F { rect.left + particle.Position().x, rect.top + particle.Position().y, rect.right + particle.Position().x, rect.bottom + particle.Position().y };

  render_target::renderTarget()->FillRectangle(particleRect, brush.get());
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

auto renderer::Render(const button& buttonObject) const -> void
{
  const auto& rect = buttonObject.Rect();
  const auto& text = buttonObject.Text();
  
  if( buttonObject.HoverState() )
  {
    text_renderer textRenderer { { m_menuBrushes.Get(menu_brushes::id::button_hover) }, { m_renderText.get(render_text::selector::menu_text_hover) } };
    textRenderer.Write(rect, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER, text.c_str());
  }
  else
  {
    text_renderer textRenderer { { m_menuBrushes.Get(menu_brushes::id::button) }, { m_renderText.get(render_text::selector::menu_text_default) } };
    textRenderer.Write(rect, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER, text.c_str());
  }
}

auto renderer::Render(const setting_slider& settingSlider) const -> void
{
  const auto& rect = settingSlider.Rect();

  column_def columnDef { rect, 2 };

  auto headerRect = columnDef[0];

  auto textBrush = settingSlider.HoverState() ? m_menuBrushes.Get(menu_brushes::id::button_hover) : m_menuBrushes.Get(menu_brushes::id::button);

  text_renderer textRenderer { { textBrush }, { m_renderText.get(render_text::selector::menu_text_small) } };
  textRenderer.Write(headerRect, DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_TEXT_ALIGNMENT_CENTER, settingSlider.Name().c_str());

  auto sliderRect = columnDef[1];

  row_def rowDef { sliderRect, static_cast<size_t>(settingSlider.Max() - settingSlider.Min())  };

  auto currentRow = settingSlider.Min();

  for( const auto& rowRect : rowDef )
  {
    if( currentRow++ < settingSlider.Value() )
    {
      render_target::renderTarget()->FillRectangle(rowRect, m_menuBrushes.Get(menu_brushes::id::button_hover).get());
    }
    else
    {
      render_target::renderTarget()->FillRectangle(rowRect, m_menuBrushes.Get(menu_brushes::id::button).get());
    }
  }
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
  render_target::renderTarget()->FillGeometry(geometry.Get(), brushSelector.Fill().get());
  render_target::renderTarget()->DrawGeometry(geometry.Get(), brushSelector.Draw().get(), brushSelector.StrokeWidth());
}

template <typename brush_selector>
auto renderer::RenderWithBorder(const transformed_path_geometry& geometry, const brush_selector& brushSelector) const -> void
{
  render_target::renderTarget()->FillGeometry(geometry.Get(), brushSelector.Fill().get());
  render_target::renderTarget()->DrawGeometry(geometry.Get(), brushSelector.Draw().get(), brushSelector.StrokeWidth());
}

auto renderer::RenderWithNoBorder(const path_geometry& geometry, ID2D1SolidColorBrush* brush) const -> void
{
  render_target::renderTarget()->FillGeometry(geometry.Get(), brush);
}

auto renderer::RenderWithNoBorder(const transformed_path_geometry& geometry, ID2D1SolidColorBrush* brush) const -> void
{
  render_target::renderTarget()->FillGeometry(geometry.Get(), brush);
}
