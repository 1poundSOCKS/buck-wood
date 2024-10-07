#pragma once

#include "default_object.h"

class default_object_collection
{

public:

  using value_type = std::list<default_object>;
  using iterator = value_type::iterator;
  using const_iterator = value_type::const_iterator;

public:

  template <typename variant_type, typename...Args> auto Add(std::in_place_type_t<variant_type> variantType, POINT_2F position, SCALE_2F scale, float angle, Args...args) -> default_object&;
  auto Visit(auto&& visitor) -> void;
  auto EraseDestroyed() -> void;

  [[nodiscard]] auto begin() noexcept -> iterator;
  [[nodiscard]] auto begin() const noexcept -> const_iterator;
  [[nodiscard]] auto end() noexcept -> iterator;
  [[nodiscard]] auto end() const noexcept -> const_iterator;

private:

  value_type m_objects;

};

template <typename variant_type, typename...Args>
auto default_object_collection::Add(std::in_place_type_t<variant_type> variantType, POINT_2F position, SCALE_2F scale, float angle, Args...args) -> default_object&
{
  return m_objects.emplace_back(variantType, position, scale, angle, std::forward<Args>(args)...);
}

auto default_object_collection::Visit(auto&& visitor) -> void
{
  for( auto& object : m_objects )
  {
    object.Visit([this, &visitor](auto& levelObject) { visitor(levelObject); });
  }
}

inline auto default_object_collection::begin() noexcept -> iterator
{
  return std::begin(m_objects);
}

inline auto default_object_collection::begin() const noexcept -> const_iterator
{
  return std::begin(m_objects);
}

inline auto default_object_collection::end() noexcept -> iterator
{
  return std::end(m_objects);
}

inline auto default_object_collection::end() const noexcept -> const_iterator
{
  return std::end(m_objects);
}
