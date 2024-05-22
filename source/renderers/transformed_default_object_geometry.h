#pragma once

#include "objects/default_object.h"
#include "objects/geometric_object.h"

inline [[nodiscard]] auto ExtractGeometricObject(const default_object& object) -> const geometric_object&
{
  const geometric_object* geometricObject { nullptr };
  std::visit([&geometricObject](auto& levelObject)
  {
    geometricObject = &levelObject;
  }, object.Get());

  return *geometricObject;
}

inline [[nodiscard]] auto ExtractLevelObject(const default_object& object)
{
  std::visit([&geometricObject](auto& levelObject)
  {
    geometricObject = &levelObject;
  }, object.Get());

  return *geometricObject;
}

class transformed_default_object_geometry
{

public:

  transformed_default_object_geometry(const default_object& object) : m_value { GetGeometry(object) }
  {
  }

  [[nodiscard]] auto Get() const noexcept -> winrt::com_ptr<ID2D1Geometry>
  {
    return m_value;
  }

  [[nodiscard]] auto GetRaw() const noexcept -> ID2D1Geometry*
  {
    return m_value.get();
  }
  
private:

  static [[nodiscard]] auto GetGeometry(const default_object& object) -> winrt::com_ptr<ID2D1Geometry>
  {
    const auto& geometricObject = ExtractGeometricObject(object);
    auto transform = geometric_object_transform { geometricObject };
    auto geometry = level_object_geometry { object };
    return direct2d::CreateTransformedGeometry(d2d_factory::get_raw(), geometry.GetRaw(), transform.Get());
  }

  winrt::com_ptr<ID2D1Geometry> m_value;
};
