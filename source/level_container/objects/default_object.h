#pragma once

#include "level_target.h"
#include "player_ship.h"
#include "mine.h"

class default_object
{

public:

  using object_type = std::variant<level_target, player_ship, mine>;

  template <typename variant_type, typename...Args> default_object(std::in_place_type_t<variant_type> variantType, Args...args) :
    m_object { variantType, std::forward<Args>(args)... }
  {
  }

  [[nodiscard]] auto Get() const -> const object_type& { return m_object; }
  [[nodiscard]] auto Get() -> object_type& { return m_object; }

  [[nodiscard]] auto Scale() const -> SCALE_2F;
  [[nodiscard]] auto Angle() const -> float;
  [[nodiscard]] auto Position() const -> D2D1_POINT_2F;
  [[nodiscard]] auto Destroyed() const -> bool;

  auto Update(float interval, std::optional<POINT_2F> playerPosition) -> void;
  auto Destroy() -> void;

private:

  object_type m_object;

};

inline [[nodiscard]] auto default_object::Scale() const -> SCALE_2F
{
  struct visitor
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
  };

  return std::visit(visitor{}, m_object);
}

inline [[nodiscard]] auto default_object::Angle() const -> float
{
  struct visitor
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
  };

  return std::visit(visitor{}, m_object);
}

inline [[nodiscard]] auto default_object::Position() const -> D2D1_POINT_2F
{
  struct visitor
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
  };

  return std::visit(visitor{}, m_object);
}

inline [[nodiscard]] auto default_object::Destroyed() const -> bool
{
  struct visitor
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
  };

  return std::visit(visitor{}, m_object);
}

inline auto default_object::Update(float interval, std::optional<POINT_2F> playerPosition) -> void
{
  struct visitor
  {
    float m_interval;
    std::optional<POINT_2F> m_playerPosition;

    auto operator()(level_target& object)
    {
      return object.Update(m_interval);
    }
    auto operator()(player_ship& object)
    {
      return object.Update(m_interval);
    }
    auto operator()(mine& object)
    {
      return object.Update(m_interval, m_playerPosition);
    }
  };

  return std::visit(visitor{ interval, playerPosition }, m_object);
}

inline auto default_object::Destroy() -> void
{
  struct visitor
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
  };

  return std::visit(visitor {}, m_object);
}
