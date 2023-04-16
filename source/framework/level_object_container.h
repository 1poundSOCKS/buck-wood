#ifndef _level_object_container_
#define _level_object_container_

#include "active_object.h"
#include "passive_object.h"

struct level_object_container
{
public:

  using active_object_collection_type = std::list<active_object>;
  using passive_object_collection_type = std::vector<passive_object>;

  level_object_container(int64_t counterFrequency);
  [[nodiscard]] auto GetActiveObjectInserter() -> std::back_insert_iterator<active_object_collection_type>;
  [[nodiscard]] auto GetOverlayObjectInserter() -> std::back_insert_iterator<passive_object_collection_type>;
  auto Update(int64_t counterValue) -> void;
  auto RenderTo(ID2D1RenderTarget* renderTarget, const D2D1::Matrix3x2F& viewTransform) -> void;
  [[nodiscard]] auto IsComplete() -> bool;
  [[nodiscard]] auto GetUpdateInterval() -> float;

private:

  [[nodiscard]] auto GetViewRect(ID2D1RenderTarget* renderTarget, const D2D1::Matrix3x2F& viewTransform) -> D2D1_RECT_F;

  const int64_t counterFrequency = 0;
  int64_t currentTimerCount = 0;
  int64_t previousTimerCount = 0;
  active_object_collection_type m_activeObjects;
  passive_object_collection_type m_overlayObjects;
};

#endif
