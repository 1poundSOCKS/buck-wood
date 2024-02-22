#pragma once

#include "level_objects.h"
#include "dynamic_object.h"

class targetted_object
{

public:

  targetted_object(dynamic_object<default_object>* object) : m_object { object }
  {
  }

  [[nodiscard]] auto Position() const -> D2D1_POINT_2F
  {
    return (*m_object)->Position();
  }

  [[nodiscard]] auto Bounds(D2D1::Matrix3x2F transform) const -> D2D1_RECT_F
  {
    auto unrotatedTransform = transform = m_object->CreateUnrotatedTransform() * transform;
    auto topLeft = unrotatedTransform.TransformPoint({-30, -30});
    auto bottomRight = unrotatedTransform.TransformPoint({30, 30});
    return { topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };
  }

  [[nodiscard]] auto Destroyed() const -> bool
  {
    return (*m_object)->Destroyed();
  }

private:

  dynamic_object<default_object>* m_object;

};
