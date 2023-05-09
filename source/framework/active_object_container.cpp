#include "pch.h"
#include "active_object_container.h"
#include "dwrite_factory.h"

active_object_container::active_object_container()
{
}

auto active_object_container::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();

  std::for_each(std::execution::seq, m_activeObjects.begin(), m_activeObjects.end(), [this](auto& object)
  {
    object.Initialize(m_renderTarget.get());
  });
}

[[nodiscard]] auto active_object_container::GetActiveObjectInserter() -> std::back_insert_iterator<active_object_collection_type>
{
  return std::back_inserter(m_activeObjects);
}

auto active_object_container::Update(const object_input_data& inputData, int64_t elapsedTicks) -> void
{
  std::for_each(std::execution::seq, m_activeObjects.begin(), m_activeObjects.end(), [&inputData, elapsedTicks](auto& object)
  {
    object.Update(inputData, elapsedTicks);
  });
}

auto active_object_container::DoCollisions() -> void
{
  std::vector<active_object_collection_type::iterator> objectIterators;
  
  for( auto objectIterator = m_activeObjects.begin(); objectIterator != m_activeObjects.end(); ++objectIterator )
  {
    objectIterators.emplace_back(objectIterator);
  }

  std::for_each(std::execution::par, objectIterators.begin(), objectIterators.end(), [this](auto objectIterator)
  {
    for( auto secondObjectIterator = std::next(objectIterator); secondObjectIterator != m_activeObjects.end(); ++secondObjectIterator)
    {
      auto& mainObject = *objectIterator;
      auto& collisionObject = *secondObjectIterator;

      const auto& collisionData1 = mainObject.GetCollisionData();
      const auto& collisionData2 = collisionObject.GetCollisionData();

      if( mainObject.HasCollidedWith(collisionData2) )
      {
        const auto& effect = collisionObject.GetCollisionEffect();
        mainObject.ApplyCollisionEffect(effect);
      }

      if( collisionObject.HasCollidedWith(collisionData1) )
      {
        const auto& effect = mainObject.GetCollisionEffect();
        collisionObject.ApplyCollisionEffect(effect);
      }
    }
  });
}

auto active_object_container::DoCollisionsWith(active_object_container& objects) -> void
{
  std::for_each(std::execution::par, m_activeObjects.begin(), m_activeObjects.end(), [&objects](auto& dynamicObject)
  {
    for( auto& staticObject : objects.m_activeObjects )
    {
      {
        const auto& collisionData = staticObject.GetCollisionData();

        if( dynamicObject.HasCollidedWith(collisionData) )
        {
          const auto& effect = staticObject.GetCollisionEffect();
          dynamicObject.ApplyCollisionEffect(effect);
        }
      }

      {
        const auto& collisionData = dynamicObject.GetCollisionData();

        if( staticObject.HasCollidedWith(collisionData) )
        {
          const auto& effect = dynamicObject.GetCollisionEffect();
          staticObject.ApplyCollisionEffect(effect);
        }
      }
    }
  });
}

auto active_object_container::Render(D2D1_RECT_F viewRect) const -> void
{
  std::for_each(std::execution::seq, m_activeObjects.cbegin(), m_activeObjects.cend(), [viewRect](const auto& object)
  {
    object.Render(viewRect);
  });
}

auto active_object_container::ClearAll() -> void
{
  m_activeObjects.clear();
}


auto active_object_container::ClearDestroyedObjects() -> void
{
  auto object = m_activeObjects.begin();
  while( object != m_activeObjects.end() )
  {
    object = object->Destroyed() ? m_activeObjects.erase(object) : ++object;
  }
}
