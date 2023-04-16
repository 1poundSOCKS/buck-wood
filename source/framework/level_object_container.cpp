#include "pch.h"
#include "level_object_container.h"
#include "game_objects.h"
#include "game_constants.h"

level_object_container::level_object_container(int64_t counterFrequency) : counterFrequency(counterFrequency)
{
}

[[nodiscard]] auto level_object_container::GetActiveObjectInserter() -> std::back_insert_iterator<active_object_collection_type>
{
  return std::back_inserter(m_activeObjects);
}

[[nodiscard]] auto level_object_container::GetOverlayObjectInserter() -> std::back_insert_iterator<passive_object_collection_type>
{
  return std::back_inserter(m_overlayObjects);
}

[[nodiscard]] auto level_object_container::IsComplete() -> bool
{
  int total = std::reduce(m_activeObjects.cbegin(), m_activeObjects.cend(), 0, [](auto count, const active_object& object)
  {
    return object.LevelIsComplete() ? count + 1 : count;
  });

  return total == m_activeObjects.size();
}

auto level_object_container::Update(int64_t counterValue) -> void
{
  previousTimerCount = currentTimerCount;
  currentTimerCount = counterValue;

  std::list<play_event> events;
  std::for_each(std::execution::seq, m_activeObjects.begin(), m_activeObjects.end(), [this, &events](auto& object)
  {
    object.Update(this->counterFrequency, this->currentTimerCount - this->previousTimerCount, std::back_inserter(events));
  });

  std::for_each(std::execution::seq, m_activeObjects.begin(), m_activeObjects.end(), [this, &events](auto& mainObject)
  {
    std::for_each(m_activeObjects.begin(), m_activeObjects.end(), [&mainObject, &events](auto& collisionObject)
    {
      auto collisionData = collisionObject.GetCollisionData();
      if( mainObject.HasCollidedWith(collisionData) )
      {
        auto effect = collisionObject.GetCollisionEffect();
        mainObject.ApplyCollisionEffect(effect, std::back_inserter(events));
      }
    });
  });

  std::for_each(events.cbegin(), events.cend(), [](auto& event)
  {
    event.Trigger();
  });

  auto object = m_activeObjects.begin();
  while( object != m_activeObjects.end() )
  {
    object = object->Destroyed() ? m_activeObjects.erase(object) : ++object;
  }

  std::for_each(std::execution::seq, m_overlayObjects.begin(), m_overlayObjects.end(), [this](auto& object)
  {
    object.Update(this->counterFrequency, this->currentTimerCount - this->previousTimerCount);
  });
}

float level_object_container::GetUpdateInterval()
{
  int64_t intervalCount = currentTimerCount - previousTimerCount;
  return static_cast<float>(intervalCount) / static_cast<float>(counterFrequency) * gameSpeedMultiplier;
}

auto level_object_container::RenderTo(ID2D1RenderTarget* renderTarget, const D2D1::Matrix3x2F& viewTransform) -> void
{
  renderTarget->SetTransform(viewTransform);

  auto viewRect = GetViewRect(renderTarget, viewTransform);

  std::for_each(std::execution::seq, m_activeObjects.cbegin(), m_activeObjects.cend(), [renderTarget, viewRect](const auto& object)
  {
    object.RenderTo(renderTarget, viewRect);
  });

  renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

  std::for_each(std::execution::seq, m_overlayObjects.cbegin(), m_overlayObjects.cend(), [renderTarget](const auto& object)
  {
    object.RenderTo(renderTarget);
  });
}

[[nodiscard]] auto level_object_container::GetViewRect(ID2D1RenderTarget* renderTarget, const D2D1::Matrix3x2F& viewTransform) -> D2D1_RECT_F
{
  auto renderTargetSize = renderTarget->GetSize();
  auto renderTargetTopLeft  = D2D1_POINT_2F { 0, 0 };
  auto renderTargetBottomRight  = D2D1_POINT_2F { renderTargetSize.width - 1.0f, renderTargetSize.height - 1.0f };

  auto invertedViewTransform = viewTransform;
  if( invertedViewTransform.Invert() )
  {
    auto topLeft = invertedViewTransform.TransformPoint(renderTargetTopLeft);
    auto bottomRight = invertedViewTransform.TransformPoint(renderTargetBottomRight);
    return { topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };
  }
  else
  {
    return { renderTargetTopLeft.x, renderTargetTopLeft.y, renderTargetBottomRight.x, renderTargetBottomRight.y };
  }
}
