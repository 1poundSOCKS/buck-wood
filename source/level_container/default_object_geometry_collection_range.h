#pragma once

#include "default_object_geometry_collection.h"

class default_object_geometry_collection_range
{

public:

  using value_type = default_object_geometry_collection;
  using iterator = value_type::iterator;
  using const_iterator = value_type::const_iterator;

  default_object_geometry_collection_range(value_type& collection);

  [[nodiscard]] auto begin() noexcept -> iterator;
  [[nodiscard]] auto begin() const noexcept -> const_iterator;
  [[nodiscard]] auto end() noexcept -> iterator;
  [[nodiscard]] auto end() const noexcept -> const_iterator;

private:

  value_type& m_collection;

};

inline default_object_geometry_collection_range::default_object_geometry_collection_range(value_type& collection) : m_collection { collection }
{
}

inline [[nodiscard]] auto default_object_geometry_collection_range::begin() noexcept -> iterator
{
  return std::begin(m_collection);
}

inline [[nodiscard]] auto default_object_geometry_collection_range::begin() const noexcept -> const_iterator
{
  return std::begin(m_collection);
}

inline [[nodiscard]] auto default_object_geometry_collection_range::end() noexcept -> iterator
{
  return std::end(m_collection);
}

inline [[nodiscard]] auto default_object_geometry_collection_range::end() const noexcept -> const_iterator
{
  return std::end(m_collection);
}
