#pragma once

#include "default_object.h"
#include "level_geometry_functions.h"
#include "pixel_geometry.h"

class level_geometries
{

public:

  static auto create() -> void;
  static auto destroy() -> void;

  static [[nodiscard]] auto get(const default_object& defaultObject) -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto get(object_type objectType) -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto get(auto&& object) -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto RectangleGeometry() -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto HudTargetGeometries() -> const std::vector<winrt::com_ptr<ID2D1Geometry>>&;

private:

  level_geometries();

  [[nodiscard]] auto Get(object_type objectType) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto GetBase(object_type objectType) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto ScaledGeometry(object_type objectType, SIZE_F geometrySize) -> winrt::com_ptr<ID2D1Geometry>;

  static [[nodiscard]] auto Scale(ID2D1Geometry* geometry, SIZE_F size) -> SCALE_2F;
  static [[nodiscard]] auto LoadHudTargetGeometries(auto&& geometryInserter) -> void;

private:

  inline static level_geometries* m_instance { nullptr };

  inline static auto m_playerPixelImage = std::array {
    std::string { "   " },
    std::string { " 0 " },
    std::string { "   " }
  };

  inline static auto m_playerPixelData = std::array {
    cell_id { 0, -1 },
    cell_id { 0, 0 },
    cell_id { 1, 0 },
    cell_id { 0, 1 },
    cell_id { -1, 0 }
  };

  winrt::com_ptr<ID2D1Geometry> m_rectangleGeometry;
  winrt::com_ptr<ID2D1Geometry> m_mineGeometry;
  winrt::com_ptr<ID2D1Geometry> m_targetGeometry;
  std::vector<winrt::com_ptr<ID2D1Geometry>> m_hudTargetGeometries;

  winrt::com_ptr<ID2D1Geometry> m_player;
  winrt::com_ptr<ID2D1Geometry> m_playerBullet;
  winrt::com_ptr<ID2D1Geometry> m_enemy1;
  winrt::com_ptr<ID2D1Geometry> m_enemy2;
  winrt::com_ptr<ID2D1Geometry> m_enemy3;
  winrt::com_ptr<ID2D1Geometry> m_enemyBullet1;
  winrt::com_ptr<ID2D1Geometry> m_portal;
  winrt::com_ptr<ID2D1Geometry> m_powerUp;

};

inline [[nodiscard]] auto level_geometries::get(const default_object& defaultObject) -> winrt::com_ptr<ID2D1Geometry>
{
  return defaultObject.Visit([](const auto& object) { return get(object); });
}

inline [[nodiscard]] auto level_geometries::get(object_type objectType) -> winrt::com_ptr<ID2D1Geometry>
{
  return m_instance->Get(objectType);
}

[[nodiscard]] auto level_geometries::get(auto&& object) -> winrt::com_ptr<ID2D1Geometry>
{
  auto objectType = level_objects::Type(object);
  return get(objectType);
}

inline [[nodiscard]] auto level_geometries::RectangleGeometry() -> winrt::com_ptr<ID2D1Geometry>
{
  return m_instance->m_rectangleGeometry;
}

inline [[nodiscard]] auto level_geometries::HudTargetGeometries() -> const std::vector<winrt::com_ptr<ID2D1Geometry>>&
{
  return m_instance->m_hudTargetGeometries;
}
