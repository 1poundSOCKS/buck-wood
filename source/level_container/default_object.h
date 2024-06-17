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
    enemy_type_3
  >;

  template <typename variant_type, typename...Args> default_object(std::in_place_type_t<variant_type> variantType, POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) :
    m_object { variantType, position, scale, angle, velocity }
  {
  }

  [[nodiscard]] auto Get() const -> const object_type& { return m_object; }
  [[nodiscard]] auto Get() -> object_type& { return m_object; }

  template <typename type> [[nodiscard]] auto GetIf() const -> const type*
  {
    return std::get_if<type>(&m_object);
  }

  template <typename type> [[nodiscard]] auto GetIf() -> type*
  {
    return std::get_if<type>(&m_object);
  }

  template <typename type> [[nodiscard]] auto GetObj() -> type&
  {
    return std::get<type>(m_object);
  }

  template <typename type> [[nodiscard]] auto HoldsAlternative() const -> bool
  {
    return std::holds_alternative<type>(m_object);
  }

  auto Visit(auto&& visitor) noexcept;
  auto Visit(const auto& visitor) const noexcept;

  [[nodiscard]] auto Scale() const -> SCALE_2F;
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Destroyed() const -> bool;

  auto Update(float interval) -> void;
  auto Destroy() -> void;

private:

  object_type m_object;

};

auto default_object::Visit(auto &&visitor) noexcept
{
  return std::visit(visitor, m_object);
}

auto default_object::Visit(const auto &visitor) const noexcept
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

inline auto default_object::Update(float interval) -> void
{
  Visit([interval](auto& object) { object.Update(interval); });
}

inline auto default_object::Destroy() -> void
{
  Visit([](auto& object) { object.Destroy(); });
}
