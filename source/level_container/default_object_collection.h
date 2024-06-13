#pragma once

#include "default_object.h"

class default_object_collection
{

public:

  using value_type = std::list<default_object>;
  using iterator = value_type::iterator;
  using const_iterator = value_type::const_iterator;

  enum object_type { portal_entry, portal_exit, player, enemy_stalker, enemy_random, enemy_turret, power_up, cell };

public:

  auto Create(auto variantType, POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) -> default_object&;
  auto Update(float interval, auto&& visitor) -> void;
  auto EraseDestroyed() -> void;

  [[nodiscard]] auto begin() noexcept -> iterator;
  [[nodiscard]] auto end() noexcept -> iterator;

  [[nodiscard]] auto begin() const noexcept -> const_iterator;
  [[nodiscard]] auto end() const noexcept -> const_iterator;

private:

  auto UpdateObject(auto &object, float interval, auto&& visitor) -> void;

private:

  value_type m_objects;

};

auto default_object_collection::Create(auto variantType, POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) -> default_object&
{
  return m_objects.emplace_back(variantType, position, scale, angle, velocity);
}

auto default_object_collection::Update(float interval, auto&& visitor) -> void
{
  for( auto& object : m_objects )
  {
    std::visit([this, interval, &visitor](auto& levelObject) { UpdateObject(levelObject, interval, visitor); }, object.Get());
  }
}

inline auto default_object_collection::begin() noexcept -> iterator
{
  return std::begin(m_objects);
}

inline auto default_object_collection::end() noexcept -> iterator
{
  return std::end(m_objects);
}

inline auto default_object_collection::begin() const noexcept -> const_iterator
{
  return std::begin(m_objects);
}

inline auto default_object_collection::end() const noexcept -> const_iterator
{
  return std::end(m_objects);
}

auto default_object_collection::UpdateObject(auto& object, float interval, auto&& visitor) -> void
{
  object.Update(interval);
  visitor(object);
}
