#pragma once

#include "level_objects.h"

class targetted_object
{

public:

  targetted_object(mine* object) : m_object { object }
  {
  }

  targetted_object(level_target* object) : m_object { object }
  {
  }

  [[nodiscard]] auto Position() const -> game_point
  {
    struct visitor
    {
      [[nodiscard]] auto operator()(const mine* object) -> game_point
      {
        return object->Position();
      }

      [[nodiscard]] auto operator()(const level_target* object) -> game_point
      {
        return object->Position();
      }
    };

    return std::visit(visitor {}, m_object);
  }

  [[nodiscard]] auto Destroyed() const -> bool
  {
    struct visitor
    {
      [[nodiscard]] auto operator()(const mine* object) -> bool
      {
        return object->Destroyed();
      }

      [[nodiscard]] auto operator()(const level_target* object) -> bool
      {
        return false;
      }
    };

    return std::visit(visitor {}, m_object);
  }

private:

  using object_type = std::variant<mine*,level_target*>;
  object_type m_object;

};
