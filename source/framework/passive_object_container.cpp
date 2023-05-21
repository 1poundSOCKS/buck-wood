#include "pch.h"
#include "passive_object_container.h"
#include "dwrite_factory.h"

passive_object_container::passive_object_container()
{
}

[[nodiscard]] auto passive_object_container::GetInserter() -> inserter_type
{
  return std::back_inserter(m_objects);
}

[[nodiscard]] auto passive_object_container::ObjectCount() -> size_t
{
  return m_objects.size();
}

auto passive_object_container::Update(const object_input_data& inputData, int64_t elapsedTicks) -> void
{
  std::for_each(std::execution::seq, m_objects.begin(), m_objects.end(), [&inputData, elapsedTicks](auto& object)
  {
    object.Update(inputData, elapsedTicks);
  });
}

auto passive_object_container::Render(D2D1_RECT_F viewRect) const -> void
{
  std::for_each(std::execution::seq, m_objects.cbegin(), m_objects.cend(), [viewRect](const auto& object)
  {
    object.Render(viewRect);
  });
}

auto passive_object_container::Clear() -> void
{
  m_objects.clear();  
}
