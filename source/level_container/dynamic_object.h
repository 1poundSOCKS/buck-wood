#pragma once

#include "framework.h"

template <typename object_type>
class dynamic_object
{

public:

  template <typename...Args> dynamic_object(ID2D1Geometry* sourceGeometry, Args...args) : 
    m_object { std::forward<Args>(args)... }, 
    m_transform { CalculateObjectTransform() },
    m_geometry { direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), sourceGeometry, m_transform) },
    m_geometryRadius { direct2d::GetGeometryRadius(m_geometry.get()) }
  {
  }

  [[nodiscard]] auto Object() -> object_type&
  {
    return m_object;
  }

  [[nodiscard]] auto Object() const -> const object_type&
  {
    return m_object;
  }

  [[nodiscard]] auto ObjectPtr() -> object_type*
  {
    return &m_object;
  }

  [[nodiscard]] auto ObjectPtr() const -> const object_type*
  {
    return &m_object;
  }

  [[nodiscard]] auto Geometry() -> ID2D1TransformedGeometry*
  {
    return m_geometry.get();
  }

  [[nodiscard]] auto Geometry() const -> ID2D1TransformedGeometry*
  {
    return m_geometry.get();
  }

  [[nodiscard]] auto GeometryRadius() const -> float
  {
    return m_geometryRadius;
  }

  [[nodiscard]] auto Transform() const -> const D2D1::Matrix3x2F&
  {
    return m_transform;
  }

  [[nodiscard]] auto CreateUnrotatedTransform() const -> D2D1::Matrix3x2F
  {
    return D2D1::Matrix3x2F::Scale(D2D1_SIZE_F { m_object.Scale().x, m_object.Scale().y }) * 
      D2D1::Matrix3x2F::Translation(m_object.Position().x, m_object.Position().y);
  }

  template <typename...Args> auto Update(Args...args)
  {
    m_object.Update(std::forward<Args>(args)...);
    m_transform = CalculateObjectTransform();
    m_geometry = direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), direct2d::GetSourceGeometry(m_geometry.get()).get(), m_transform);
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

  [[nodiscard]] auto CalculateObjectTransform() const -> D2D1::Matrix3x2F
  {
    return D2D1::Matrix3x2F::Scale(D2D1_SIZE_F { m_object.Scale().x, m_object.Scale().y }) * 
      D2D1::Matrix3x2F::Rotation(m_object.Angle()) * 
      D2D1::Matrix3x2F::Translation(m_object.Position().x, m_object.Position().y);
  }

private:

  object_type m_object;
  D2D1::Matrix3x2F m_transform;
  winrt::com_ptr<ID2D1TransformedGeometry> m_geometry;
  float m_geometryRadius;

};
