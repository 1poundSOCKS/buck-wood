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
  // levelContainer.EnumerateFloorCollisionObjects([this](const collision_object& object){
  //   auto geometry = object.Geometry().GetRaw();
  //   m_floorCellRenderer.Write(geometry);
  // });

  levelContainer.EnumerateCellObjects(level_cell_type::floor, [this](const auto& object)
  {
    Write(object);
  });

  levelContainer.EnumerateNonInteractiveObjects([this](const auto& object)
  {
    Write(object);
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
