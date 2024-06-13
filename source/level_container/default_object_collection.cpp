#include "pch.h"
#include "default_object_collection.h"

auto default_object_collection::Update(float interval, auto&& visitor) -> void
{
  for( auto& object : m_objects )
  {
    std::visit([this, interval](auto& levelObject) { UpdateObject(levelObject, interval, visitor); }, object.Get());
  }
}
