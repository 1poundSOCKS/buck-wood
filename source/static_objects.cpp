#include "pch.h"
#include "static_objects.h"

static_objects::static_objects()
{
  m_objectContainers.resize(2);
}

auto static_objects::GetObjectContainers() -> active_object_container_collection&
{
  return m_objectContainers;
}

auto static_objects::AddTarget(level_target target) -> void
{
  m_objectContainers.front().AppendActiveObject(target);
}

auto static_objects::AddAsteroid(level_asteroid asteroid) -> void
{
  m_objectContainers.back().AppendActiveObject(asteroid);
}

auto static_objects::SetCentre(float x, float y) -> void
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
