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

auto static_objects::Initialize() -> void
{
  for( auto& objectContainer : m_objectContainers )
  {
    objectContainer.Initialize(framework::renderTarget().get());
  }
}

auto static_objects::Update(const object_input_data& inputData, int64_t ticks) -> void
{
  GetAsteroids().ClearAll();

  auto smallColumnWidth = 100;
  auto smallRowHeight = 100;

  auto smallCentreColumn = m_x / smallColumnWidth;
  auto smallCentreRow = m_y / smallRowHeight;

  game_level_object_generator smallAsteroidGenerator(smallCentreColumn - 17, smallCentreColumn + 16, smallColumnWidth, smallCentreRow - 10, smallCentreRow + 10, smallRowHeight, 0.88f, 0.94f, 13.0f);
  GenerateAsteroids(smallAsteroidGenerator, GetAsteroids());

  auto largeColumnWidth = 400;
  auto largeRowHeight = 400;

  auto largeCentreColumn = m_x / largeColumnWidth;
  auto largeCentreRow = m_y / largeRowHeight;

  game_level_object_generator largeAsteroidGenerator(largeCentreColumn - 4, largeCentreColumn + 5, largeColumnWidth, largeCentreRow - 4, largeCentreRow + 4, largeRowHeight, 0.94f, 1.0f, 7.0f);
  GenerateAsteroids(largeAsteroidGenerator, GetAsteroids());

  for( auto& objectContainer : m_objectContainers )
  {
    objectContainer.Update(inputData, ticks);
  }
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

auto static_objects::GenerateAsteroids(const game_level_object_generator& generator, active_object_container_type& objects) -> void
{
  std::vector<game_closed_object> asteroids;
  generator.InsertInto(std::back_inserter(asteroids));

  for( const auto& asteroid : asteroids )
  {
    objects.AppendActiveObject( level_asteroid { asteroid } );
  }
}
