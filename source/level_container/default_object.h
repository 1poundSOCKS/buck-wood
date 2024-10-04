#pragma once

#include "level_objects.h"

class default_object
{

public:

  using object_type = std::variant<
    level_cell, 
    player_ship, 
    player_bullet, 
    enemy_bullet_1, 
    power_up, 
    portal, 
    enemy_type_1, 
    enemy_type_2, 
    enemy_type_3,
    boundary_walls
  >;

  template <typename variant_type, typename...Args>
  default_object(std::in_place_type_t<variant_type> variantType, POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity, Args...args);

  template <typename type> [[nodiscard]] auto GetIf() const -> const type*;
  template <typename type> [[nodiscard]] auto GetIf() -> type*;
  template <typename type> [[nodiscard]] auto GetObj() -> type&;
  template <typename type> [[nodiscard]] auto HoldsAlternative() const -> bool;

  auto Visit(auto&& visitor) noexcept;
  auto Visit(auto&& visitor) const noexcept;

  [[nodiscard]] auto Scale() const -> SCALE_2F;
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Destroyed() const -> bool;

  auto Destroy() -> void;

private:

  object_type m_object;

};

template <typename variant_type, typename...Args>
default_object::default_object(std::in_place_type_t<variant_type> variantType, POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity, Args...args) :
  m_object { variantType, position, scale, angle, velocity, std::forward<Args>(args)... }
{
}

template <typename type>
[[nodiscard]] auto default_object::GetIf() const -> const type*
{
  return std::get_if<type>(&m_object);
}

template <typename type>
[[nodiscard]] auto default_object::GetIf() -> type*
{
  return std::get_if<type>(&m_object);
}

template <typename type>
[[nodiscard]] auto default_object::GetObj() -> type&
{
  return std::get<type>(m_object);
}

template <typename type>
[[nodiscard]] auto default_object::HoldsAlternative() const -> bool
{
  return std::holds_alternative<type>(m_object);
}

auto default_object::Visit(auto&& visitor) noexcept
{
  return std::visit(visitor, m_object);
}

auto default_object::Visit(auto&& visitor) const noexcept
{
  return std::visit(visitor, m_object);
}

inline [[nodiscard]] auto default_object::Scale() const -> SCALE_2F
{
  return Visit([](const auto& object) { return object.Scale(); });
}

inline [[nodiscard]] auto default_object::Angle() const -> float
{
  return Visit([](const auto& object) { return object.Angle(); });
}

inline [[nodiscard]] auto default_object::Position() const -> D2D1_POINT_2F
{
  return Visit([](const auto& object) { return object.Position(); });
}

inline [[nodiscard]] auto default_object::Destroyed() const -> bool
{
  return Visit([](const auto& object) { return object.Destroyed(); });
}

inline auto default_object::Destroy() -> void
{
  Visit([](auto& object) { object.Destroy(); });
}
