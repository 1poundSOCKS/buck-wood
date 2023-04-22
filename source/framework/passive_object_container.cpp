#include "pch.h"
#include "passive_object_container.h"

passive_object_container::passive_object_container()
{
}

auto passive_object_container::Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();

  m_dwriteFactory.attach(dwriteFactory);
  m_dwriteFactory->AddRef();

  std::for_each(std::execution::seq, m_overlayObjects.begin(), m_overlayObjects.end(), [this, renderTarget, dwriteFactory](auto& object)
  {
    object.Initialize(renderTarget, dwriteFactory);
  });
}

[[nodiscard]] auto passive_object_container::GetOverlayObjectInserter() -> std::back_insert_iterator<passive_object_collection_type>
{
  return std::back_inserter(m_overlayObjects);
}

auto passive_object_container::Update(const object_input_data& inputData, int64_t elapsedTicks) -> void
{
  std::for_each(std::execution::seq, m_overlayObjects.begin(), m_overlayObjects.end(), [&inputData, elapsedTicks](auto& object)
  {
    object.Update(inputData, elapsedTicks);
  });
}

auto passive_object_container::Render(D2D1_RECT_F viewRect) const -> void
{
  std::for_each(std::execution::seq, m_overlayObjects.cbegin(), m_overlayObjects.cend(), [viewRect](const auto& object)
  {
    object.Render(viewRect);
  });
}

auto passive_object_container::Clear() -> void
{
  m_overlayObjects.clear();  
}
