#pragma once

#include "active_object.h"

class active_object_container
{
public:

  using active_object_collection_type = std::list<active_object>;

  active_object_container();
  auto Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void;

  template <typename object_type> auto AppendActiveObject(object_type& object) -> void;
  template <typename object_type> auto AppendActiveObject(object_type&& object) -> void;

  auto Update(int64_t elapsedTicks) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto IsComplete() -> bool;
  auto Clear() -> void;

private:

  [[nodiscard]] auto GetActiveObjectInserter() -> std::back_insert_iterator<active_object_collection_type>;

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<IDWriteFactory> m_dwriteFactory;
  active_object_collection_type m_activeObjects;
};

template <typename object_type> auto active_object_container::AppendActiveObject(object_type& object) -> void
{
  object.Initialize(m_renderTarget.get(), m_dwriteFactory.get());
  m_activeObjects.emplace_back(object);
}

template <typename object_type> auto active_object_container::AppendActiveObject(object_type&& object) -> void
{
  object.Initialize(m_renderTarget.get(), m_dwriteFactory.get());
  m_activeObjects.emplace_back(object);
}
