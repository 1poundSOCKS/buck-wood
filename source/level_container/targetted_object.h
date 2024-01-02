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

  [[nodiscard]] auto Position() const -> game_point
  {
    struct visitor
    {
      [[nodiscard]] auto operator()(const mine_object* object) -> game_point
      {
        return object->Object().Position();
      }

      [[nodiscard]] auto operator()(const target_object* object) -> game_point
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
        auto transform = D2D1::Matrix3x2F::Scale({object->Object().Scale().width(), object->Object().Scale().height()}) * D2D1::Matrix3x2F::Translation({object->Object().Position().x, object->Object().Position().y});
        transformed_path_geometry geometry { object->Geometry().GetSource().get(), transform };
        D2D1_RECT_F bounds;
        geometry.Get()->GetBounds(m_transform, &bounds);
        return Bounds(bounds);
      }

      [[nodiscard]] auto operator()(const target_object* object) -> D2D1_RECT_F
      {
        auto transform = D2D1::Matrix3x2F::Scale({object->Object().Scale().width(), object->Object().Scale().height()}) * D2D1::Matrix3x2F::Translation({object->Object().Position().x, object->Object().Position().y});
        transformed_path_geometry geometry { object->Geometry().GetSource().get(), transform };
        D2D1_RECT_F bounds;
        geometry.Get()->GetBounds(m_transform, &bounds);
        return Bounds(bounds);
      }

      static [[nodiscard]] auto Bounds(D2D1_RECT_F rect) -> D2D1_RECT_F
      {
        auto size = Size(rect);
        auto widthAdjustment = size.width * 0.2f;
        auto heightAdjustment = size.height * 0.2f;
        return { rect.left - widthAdjustment, rect.top - heightAdjustment, rect.right + widthAdjustment, rect.bottom + heightAdjustment };
      }

      static [[nodiscard]] auto Size(D2D1_RECT_F rect) -> D2D1_SIZE_F
      {
        return { rect.right - rect.left, rect.bottom - rect.top };
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
