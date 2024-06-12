#include "pch.h"
#include "default_object_geometry_collection.h"

default_object_geometry_collection::default_object_geometry_collection(size_t initialCapacity)
{
  m_geometries.reserve(initialCapacity);
}
