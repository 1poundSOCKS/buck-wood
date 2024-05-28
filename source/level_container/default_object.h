#pragma once

#include "level_objects.h"
#include "default_object_visitors.h"

class default_object
{

public:

  using object_type = std::variant<level_wall, player_ship, player_bullet, enemy_bullet_1, power_up, portal, enemy_type_1, enemy_type_2, enemy_type_3>;

  // template <typename variant_type, typename...Args> default_object(std::in_place_type_t<variant_type> variantType, Args...args) :
  //   m_object { variantType, std::forward<Args>(args)... }
  // {
  // }
  template <typename variant_type, typename...Args> default_object(std::in_place_type_t<variant_type> variantType, POINT_2F position, SCALE_2F scale, float angle) :
    m_object { variantType, position, scale, angle }
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

  template <typename type> [[nodiscard]] auto HoldsAlternative() const -> bool
  {
    return std::holds_alternative<type>(m_object);
  }

  [[nodiscard]] auto Scale() const -> SCALE_2F;
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Destroyed() const -> bool;
  [[nodiscard]] auto Health() const -> float;

  auto Destroy() -> void;

private:

  object_type m_object;

};

inline [[nodiscard]] auto default_object::Scale() const -> SCALE_2F
{
  return std::visit(scale_visitor {}, m_object);
}

inline [[nodiscard]] auto default_object::Angle() const -> float
{
  return std::visit(angle_visitor{}, m_object);
}

inline [[nodiscard]] auto default_object::Position() const -> D2D1_POINT_2F
{
  return std::visit(position_visitor {}, m_object);
}

inline [[nodiscard]] auto default_object::Destroyed() const -> bool
{
  return std::visit(destroyed_visitor{}, m_object);
}

inline auto default_object::Destroy() -> void
{
  return std::visit(destroy_visitor {}, m_object);
}

inline [[nodiscard]] auto default_object::Health() const -> float
{
  return std::visit(health_visitor {}, m_object);
}
