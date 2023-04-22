#pragma once

#include "passive_object.h"
#include "object_input_data.h"

class passive_object_container
{
public:

  using passive_object_collection_type = std::vector<passive_object>;

  passive_object_container();
  auto Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void;

  template <typename object_type> auto AppendOverlayObject(object_type& object) -> void;
  template <typename object_type> auto AppendOverlayObject(object_type&& object) -> void;

  auto Update(const object_input_data& inputData, int64_t elapsedTicks) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  auto Clear() -> void;

private:

  [[nodiscard]] auto GetOverlayObjectInserter() -> std::back_insert_iterator<passive_object_collection_type>;

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<IDWriteFactory> m_dwriteFactory;
  passive_object_collection_type m_overlayObjects;
};

template <typename object_type> auto passive_object_container::AppendOverlayObject(object_type& object) -> void
{
  object.Initialize(m_renderTarget.get(), m_dwriteFactory.get());
  m_overlayObjects.emplace_back(object);
}

template <typename object_type> auto passive_object_container::AppendOverlayObject(object_type&& object) -> void
{
  object.Initialize(m_renderTarget.get(), m_dwriteFactory.get());
  m_overlayObjects.emplace_back(object);
}
