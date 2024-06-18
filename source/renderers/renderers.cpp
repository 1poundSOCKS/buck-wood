#include "pch.h"
#include "renderers.h"

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
  int renderIndex = 0;

  while( renderIndex < render_order::particle_end() )
  {
    levelContainer.EnumerateObjects([this,renderIndex](const auto& object)
    {
      if( render_order::get(object) == renderIndex )
      {
        Write(object);
      }
    });

    ++renderIndex;
  }

  levelContainer.EnumerateParticles([this](const auto& particle)
  {
    Render(particle);
  });

  while( renderIndex < render_order::end() )
  {
    levelContainer.EnumerateObjects([this,renderIndex](const auto& object)
    {
      if( render_order::get(object) == renderIndex )
      {
        Write(object);
      }
    });

    ++renderIndex;
  }
}
