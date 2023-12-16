#pragma once

#include "dynamic_object.h"

template <typename object_type>
class dynamic_object_collection
{

public:

  using collection_type = std::vector<dynamic_object<object_type>>;

  template <typename...Args>auto Create(Args...args) -> void
  {
    m_objectCollection.emplace_back(std::forward<Args>(args)...);
  }

  template <typename...Args>auto Update(Args...args) -> void
  {
    for( auto& object : m_objectCollection )
    {
      object.Update(std::forward<Args>(args)...);
    }
  }

  [[nodiscard]] auto begin() const -> collection_type::const_iterator
  {
    return std::begin(m_objectCollection);
  }

  [[nodiscard]] auto end() const -> collection_type::const_iterator
  {
    return std::end(m_objectCollection);
  }

  [[nodiscard]] auto begin() -> collection_type::iterator
  {
    return std::begin(m_objectCollection);
  }

  [[nodiscard]] auto end() -> collection_type::iterator
  {
    return std::end(m_objectCollection);
  }

private:

  collection_type m_objectCollection;

};
