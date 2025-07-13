#include "pch.h"
#include "renderers.h"
#include "render_object_collection.h"

auto renderer::create() -> void
{
  destroy();
  m_instance = new renderer();
}

auto renderer::destroy() -> void
{
  delete m_instance;
  m_instance = nullptr;
}

auto renderer::Render(const hud_target& hudTarget) const -> void
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

auto renderer::Render(const level_container &levelContainer) const -> void
{
  render_object_collection renderObjects { levelContainer.Objects() };

  for( const auto& renderLayer : renderObjects )
  {
    for( const auto& renderObject : renderLayer )
    {
      Write(*renderObject);
    };
  }

  for( auto&& particle : levelContainer.Particles() )
  {
    Render(particle);
  }
}

auto renderer::Render(const static_asteroid &object) const -> void
{
  auto transformedGeometry = transformed_level_object_geometry { object };
  m_boundaryRenderer.Write(transformedGeometry.GetRaw());
}

auto renderer::Render(const energy_bar& energyBar) const -> void
{
  auto fillRect = energyBar.position;
  fillRect.right = fillRect.left + ( fillRect.right - fillRect.left ) * energyBar.value;
  render_target::get()->FillRectangle(fillRect, m_energyBarFillBrush.get());
  render_target::get()->DrawRectangle(energyBar.position, m_energyBarBorderBrush.get(), 5);
}

auto renderer::Write(const default_object &object) const -> void
{
  object.Visit([this](const auto& object) { Write(object); });

  auto health = Health(object);

  if( health )
  {
    transformed_default_object_geometry geometry { object };
    auto energyBarRect = energy_bar_rect { geometry.Bounds() };
    auto energyBar = energy_bar { energyBarRect.Get(), *health };
    Render(energyBar);
  }
}

auto renderer::Write(const play_state &playState) const -> void
{
  m_playStateRenderer.Write(playState);
}
