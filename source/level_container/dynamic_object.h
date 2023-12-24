#pragma once

#include "dynamic_geometry.h"

template <typename object_type>
class dynamic_object
{

public:

  template <typename...Args> dynamic_object(const path_geometry& baseGeometry, Args...args) : 
    m_object { std::forward<Args>(args)... }, m_geometry { baseGeometry, D2D1::Matrix3x2F::Translation(m_object.Position().x, m_object.Position().y) }
  {
  }

  [[nodiscard]] auto Object() -> object_type&
  {
    return m_object;
  }

  [[nodiscard]] auto Geometry() const -> const dynamic_geometry&
  {
    return m_geometry;
  }

  template <typename...Args> auto Update(Args...args)
  {
    m_object.Update(std::forward<Args>(args)...);
    m_geometry.Transform(D2D1::Matrix3x2F::Scale(D2D1_SIZE_F { m_object.Scale().width(), m_object.Scale().height() }) * 
      D2D1::Matrix3x2F::Rotation(m_object.Angle()) * 
      D2D1::Matrix3x2F::Translation(m_object.Position().x, m_object.Position().y));
  }

  auto operator->() const -> const object_type*
  {
    return &m_object;
  }

  auto operator->() -> object_type*
  {
    return &m_object;
  }

private:

  object_type m_object;
  dynamic_geometry m_geometry;

};
