#pragma once

#include "player_ship.h"
#include "player_bullet.h"
#include "level_target.h"
#include "mine.h"

struct scale_visitor
{
  auto operator()(const level_target& object)
  {
    return object.Scale();
  }
  auto operator()(const player_ship& object)
  {
    return object.Scale();
  }
  auto operator()(const mine& object)
  {
    return object.Scale();
  }
  auto operator()(const auto& object)
  {
    return object.Scale();
  }
};

struct angle_visitor
{
  auto operator()(const level_target& object)
  {
    return object.Angle();
  }
  auto operator()(const player_ship& object)
  {
    return object.Angle();
  }
  auto operator()(const mine& object)
  {
    return object.Angle();
  }
  auto operator()(const auto& object)
  {
    return object.Angle();
  }
};

struct position_visitor
{
  auto operator()(const level_target& object)
  {
    return object.Position();
  }
  auto operator()(const player_ship& object)
  {
    return object.Position();
  }
  auto operator()(const mine& object)
  {
    return object.Position();
  }
  auto operator()(const auto& object)
  {
    return object.Position();
  }
};

struct destroyed_visitor
{
  auto operator()(const level_target& object)
  {
    return object.Destroyed();
  }
  auto operator()(const player_ship& object)
  {
    return object.Destroyed();
  }
  auto operator()(const mine& object)
  {
    return object.Destroyed();
  }
  auto operator()(const auto& object)
  {
    return object.Destroyed();
  }
};

// struct update_visitor
// {
//   float m_interval;
//   std::optional<POINT_2F> m_playerPosition;

//   auto operator()(level_target& object)
//   {
//     return object.Update(m_interval, m_playerPosition);
//   }
//   auto operator()(player_ship& object)
//   {
//     return object.Update(m_interval);
//   }
//   auto operator()(mine& object)
//   {
//     return object.Update(m_interval, m_playerPosition);
//   }
//   auto operator()(auto& object)
//   {
//     return object.Update(m_interval);
//   }
// };

struct destroy_visitor
{
  auto operator()(level_target& object)
  {
  }
  auto operator()(player_ship& object)
  {
    return object.Destroy();
  }
  auto operator()(mine& object)
  {
    return object.Destroy();
  }
  auto operator()(auto& object)
  {
    return object.Destroy();
  }
};

class default_object
{

public:

  using object_type = std::variant<player_ship, player_bullet, level_target, mine>;

  template <typename variant_type, typename...Args> default_object(std::in_place_type_t<variant_type> variantType, Args...args) :
    m_object { variantType, std::forward<Args>(args)... }
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

// auto default_object::Update(float interval, std::optional<POINT_2F> playerPosition, auto&&...args) -> void
// {
//   return std::holds_alternative<level_target>(m_object) ?
//     std::get<level_target>(m_object).Update(interval, playerPosition, std::forward<decltype(args)>(args)...) :
//     std::visit(update_visitor { interval, playerPosition }, m_object);
// }

inline auto default_object::Destroy() -> void
{
  return std::visit(destroy_visitor {}, m_object);
}
