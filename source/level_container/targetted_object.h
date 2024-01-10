#pragma once

#include "level_objects.h"
#include "dynamic_object.h"

class targetted_object
{

public:

  using mine_object = dynamic_object<mine>;
  using target_object = dynamic_object<level_target>;

  targetted_object(mine_object* object) : m_object { object }
  {
  }

  targetted_object(target_object* object) : m_object { object }
  {
  }

  [[nodiscard]] auto Position() const -> D2D1_POINT_2F
  {
    struct visitor
    {
      [[nodiscard]] auto operator()(const mine_object* object) -> D2D1_POINT_2F
      {
        return object->Object().Position();
      }

      [[nodiscard]] auto operator()(const target_object* object) -> D2D1_POINT_2F
      {
        return object->Object().Position();
      }
    };

    return std::visit(visitor {}, m_object);
  }

  [[nodiscard]] auto Bounds(D2D1::Matrix3x2F transform) const -> D2D1_RECT_F
  {
    struct visitor
    {
      visitor(D2D1::Matrix3x2F transform) : m_transform { transform } {}

      [[nodiscard]] auto operator()(const mine_object* object) -> D2D1_RECT_F
      {
        auto transform = object->CreateUnrotatedTransform() * m_transform;
        auto topLeft = transform.TransformPoint({-30, -30});
        auto bottomRight = transform.TransformPoint({30, 30});
        return { topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };
      }

      [[nodiscard]] auto operator()(const target_object* object) -> D2D1_RECT_F
      {
        auto transform = object->CreateUnrotatedTransform() * m_transform;
        auto topLeft = transform.TransformPoint({-70, -70});
        auto bottomRight = transform.TransformPoint({70, 70});
        return { topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };
      }

      D2D1::Matrix3x2F m_transform;
    };

    return std::visit(visitor { transform }, m_object);
  }

  [[nodiscard]] auto Destroyed() const -> bool
  {
    struct visitor
    {
      [[nodiscard]] auto operator()(const mine_object* object) -> bool
      {
        return object->Object().Destroyed();
      }

      [[nodiscard]] auto operator()(const target_object* object) -> bool
      {
        return false;
      }
    };

    return std::visit(visitor {}, m_object);
  }

private:

  using object_type = std::variant<mine_object*,target_object*>;
  object_type m_object;

};
