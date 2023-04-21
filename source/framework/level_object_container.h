#ifndef _level_object_container_
#define _level_object_container_

#include "active_object.h"
#include "passive_object.h"

class level_object_container
{
public:

  using active_object_collection_type = std::list<active_object>;
  using passive_object_collection_type = std::vector<passive_object>;

  level_object_container();
  auto Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void;

  template <typename object_type> auto AppendActiveObject(object_type& object) -> void;
  template <typename object_type> auto AppendActiveObject(object_type&& object) -> void;
  template <typename object_type> auto AppendOverlayObject(object_type& object) -> void;
  template <typename object_type> auto AppendOverlayObject(object_type&& object) -> void;

  auto Update(int64_t elapsedTicks) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto IsComplete() -> bool;
  auto Clear() -> void;

private:

  [[nodiscard]] auto GetActiveObjectInserter() -> std::back_insert_iterator<active_object_collection_type>;
  [[nodiscard]] auto GetOverlayObjectInserter() -> std::back_insert_iterator<passive_object_collection_type>;

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<IDWriteFactory> m_dwriteFactory;
  active_object_collection_type m_activeObjects;
  passive_object_collection_type m_overlayObjects;
};

template <typename object_type> auto level_object_container::AppendActiveObject(object_type& object) -> void
{
  object.Initialize(m_renderTarget.get(), m_dwriteFactory.get());
  m_activeObjects.emplace_back(object);
}

template <typename object_type> auto level_object_container::AppendActiveObject(object_type&& object) -> void
{
  object.Initialize(m_renderTarget.get(), m_dwriteFactory.get());
  m_activeObjects.emplace_back(object);
}

template <typename object_type> auto level_object_container::AppendOverlayObject(object_type& object) -> void
{
  object.Initialize(m_renderTarget.get(), m_dwriteFactory.get());
  m_overlayObjects.emplace_back(object);
}

template <typename object_type> auto level_object_container::AppendOverlayObject(object_type&& object) -> void
{
  object.Initialize(m_renderTarget.get(), m_dwriteFactory.get());
  m_overlayObjects.emplace_back(object);
}

#endif
