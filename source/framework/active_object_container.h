#pragma once

#include "active_object.h"
#include "object_input_data.h"

template <typename collision_data_type, typename collision_effect_type>
class active_object_container
{
public:

  using active_object_type = active_object<collision_data_type, collision_effect_type>;
  using active_object_collection_type = std::list<active_object_type>;

  active_object_container();

  template <typename object_type> auto AppendActiveObject(object_type& object) -> void;
  template <typename object_type> auto AppendActiveObject(object_type&& object) -> void;

  auto Update(const object_input_data& inputData, int64_t elapsedTicks) -> void;
  auto DoCollisions() -> void;
  auto DoCollisionsWith(active_object_container<collision_data_type, collision_effect_type>& objects) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  auto ClearDestroyedObjects() -> void;
  auto ClearAll() -> void;

private:

  [[nodiscard]] auto GetActiveObjectInserter() -> std::back_insert_iterator<active_object_collection_type>;

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  active_object_collection_type m_activeObjects;
};

template <typename collision_data_type, typename collision_effect_type>
template <typename object_type> auto active_object_container<collision_data_type, collision_effect_type>::AppendActiveObject(object_type& object) -> void
{
  m_activeObjects.emplace_back(object);
}

template <typename collision_data_type, typename collision_effect_type>
template <typename object_type> auto active_object_container<collision_data_type, collision_effect_type>::AppendActiveObject(object_type&& object) -> void
{
  m_activeObjects.emplace_back(object);
}

template <typename collision_data_type, typename collision_effect_type>
active_object_container<collision_data_type, collision_effect_type>::active_object_container()
{
}

template <typename collision_data_type, typename collision_effect_type>
[[nodiscard]] auto active_object_container<collision_data_type, collision_effect_type>::GetActiveObjectInserter() -> std::back_insert_iterator<active_object_collection_type>
{
  return std::back_inserter(m_activeObjects);
}

template <typename collision_data_type, typename collision_effect_type>
auto active_object_container<collision_data_type, collision_effect_type>::Update(const object_input_data& inputData, int64_t elapsedTicks) -> void
{
  std::for_each(std::execution::seq, m_activeObjects.begin(), m_activeObjects.end(), [&inputData, elapsedTicks](auto& object)
  {
    object.Update(inputData, elapsedTicks);
  });
}

template <typename collision_data_type, typename collision_effect_type>
auto active_object_container<collision_data_type, collision_effect_type>::DoCollisions() -> void
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

template <typename collision_data_type, typename collision_effect_type>
auto active_object_container<collision_data_type, collision_effect_type>::DoCollisionsWith(active_object_container<collision_data_type, collision_effect_type>& objects) -> void
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

template <typename collision_data_type, typename collision_effect_type>
auto active_object_container<collision_data_type, collision_effect_type>::Render(D2D1_RECT_F viewRect) const -> void
{
  std::for_each(std::execution::seq, m_activeObjects.cbegin(), m_activeObjects.cend(), [viewRect](const auto& object)
  {
    object.Render(viewRect);
  });
}

template <typename collision_data_type, typename collision_effect_type>
auto active_object_container<collision_data_type, collision_effect_type>::ClearAll() -> void
{
  m_activeObjects.clear();
}

template <typename collision_data_type, typename collision_effect_type>
auto active_object_container<collision_data_type, collision_effect_type>::ClearDestroyedObjects() -> void
{
  auto object = m_activeObjects.begin();
  while( object != m_activeObjects.end() )
  {
    object = object->Destroyed() ? m_activeObjects.erase(object) : ++object;
  }
}
