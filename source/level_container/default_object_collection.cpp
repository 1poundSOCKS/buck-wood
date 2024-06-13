#include "pch.h"
#include "default_object_collection.h"

auto default_object_collection::Update(float interval) -> void
{
  for( auto& object : m_objects )
  {
    object.Update(interval);
  }
}

auto default_object_collection::EraseDestroyed() -> void
{
  std::erase_if(m_objects, [](const auto& object) -> bool { return object.Destroyed(); });
}
