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
#ifdef _RENDER_LEVEL_GRID
  levelContainer.EnumerateColumns([this](level_cell_collection::column_def columnDef)
  {
    Write(columnDef);
  });

  levelContainer.EnumerateRows([this](level_cell_collection::row_def rowDef)
  {
    Write(rowDef);
  });
#endif

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
