#include "pch.h"
#include "level_container.h"

[[nodiscard]] auto level_container::GetObjectContainer() const -> const active_object_container&
{
  return m_objectContainer;
}

[[nodiscard]] auto level_container::GetObjectContainer() -> active_object_container&
{
  return m_objectContainer;
}

auto level_container::SetTimeout(int time) -> void
{
  m_timeoutTicks = performance_counter::QueryFrequency() * time;
}

auto level_container::Update(const object_input_data& inputData, int64_t ticks) -> void
{
  m_objectContainer.Update(inputData, ticks);
  m_objectContainer.DoCollisions();
  m_objectContainer.ClearDestroyedObjects();

  m_timeoutTicks -= ticks;
  m_timeoutTicks = max(0, m_timeoutTicks);

  if( m_timeoutTicks == 0 )
  {
    m_timeoutEvent();
  }
}
