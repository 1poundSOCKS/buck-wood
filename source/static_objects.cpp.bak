#include "pch.h"
#include "static_objects.h"

static_objects::static_objects()
{
  m_objectContainers.resize(2);
}

[[nodiscard]] auto static_objects::GetObjectContainers() -> active_object_container_collection&
{
  return m_objectContainers;
}

[[nodiscard]] auto static_objects::GetTargets() -> active_object_container_type&
{
  return m_objectContainers.front();
}

[[nodiscard]] auto static_objects::GetAsteroids() -> active_object_container_type&
{
  return m_objectContainers.back();
}

auto static_objects::SetCentre(int x, int y) -> void
{
  m_x = x;
  m_y = y;
}

auto static_objects::Update(const object_input_data& inputData, int64_t ticks, D2D1_RECT_F viewRect) -> void
{
  GetAsteroids().ClearAll();

  auto smallColumnWidth = 100;
  auto smallRowHeight = 100;

  auto smallLeftColumn = static_cast<int>(viewRect.left) / smallColumnWidth - 1;
  auto smallRightColumn = static_cast<int>(viewRect.right) / smallColumnWidth + 1;
  auto smallTopRow = static_cast<int>(viewRect.top) / smallRowHeight - 1;
  auto smallBottomRow = static_cast<int>(viewRect.bottom) / smallRowHeight + 1;

  game_level_object_generator smallAsteroidGenerator(smallLeftColumn, smallRightColumn, smallColumnWidth, smallTopRow, smallBottomRow, smallRowHeight, 0.85f, 0.89f, 13.0f);
  smallAsteroidGenerator.InsertAsteroidsInto(GetAsteroids().GetInserter());

  auto largeColumnWidth = 400;
  auto largeRowHeight = 400;

  auto largeLeftColumn = static_cast<int>(viewRect.left) / largeColumnWidth - 1;
  auto largeRightColumn = static_cast<int>(viewRect.right) / largeColumnWidth + 1;
  auto largeTopRow = static_cast<int>(viewRect.top) / largeRowHeight - 1;
  auto largeBottomRow = static_cast<int>(viewRect.bottom) / largeRowHeight + 1;

  game_level_object_generator largeAsteroidGenerator(largeLeftColumn, largeRightColumn, largeColumnWidth, largeTopRow, largeBottomRow, largeRowHeight, 0.90f, 1.0f, 7.0f);
  largeAsteroidGenerator.InsertAsteroidsInto(GetAsteroids().GetInserter());
}

auto static_objects::DoCollisionsWith(active_object_container_type& objectContainer) -> void
{
  for( auto& thisObjectContainer : m_objectContainers )
  {
    thisObjectContainer.DoCollisionsWith(objectContainer);
  }
}

auto static_objects::Render(D2D1_RECT_F viewRect) const -> void
{
  for( auto& objectContainer : m_objectContainers )
  {
    objectContainer.Render(viewRect);
  }
}
