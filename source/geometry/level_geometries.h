#pragma once

#include "default_object.h"
#include "level_geometry_functions.h"
#include "pixel_geometry_loader.h"
#include "geometry/point_data.h"

class level_geometries
{

public:

  static auto create() -> void;
  static auto destroy() -> void;

  static [[nodiscard]] auto get(const default_object& defaultObject) -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto get(auto&& object) -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto getPlayerThrust() -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto RectangleGeometry() -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto HudTargetGeometries() -> const std::vector<winrt::com_ptr<ID2D1Geometry>>&;

private:

  enum class alignment { centred, below };

  level_geometries();

  [[nodiscard]] auto Get(const player_ship &object) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto Get(const player_bullet &object) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto Get(const enemy_ship &object) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto Get(const enemy_bullet &object) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto Get(const power_up &object) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto Get(const boundary_walls &object) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto Get(auto&& object) -> winrt::com_ptr<ID2D1Geometry>;

  static [[nodiscard]] auto Scale(ID2D1Geometry* geometry, SIZE_F size) -> SCALE_2F;
  static [[nodiscard]] auto LoadHudTargetGeometries(auto&& geometryInserter) -> void;

  // static [[nodiscard]] auto LoadAndCentreGeometryData(auto pixelDataBegin, auto pixelDataEnd, int pixelSize, auto&& pointDataInserter) -> void;
  static [[nodiscard]] auto LoadAndCentreGeometryData(std::ranges::input_range auto&& pixelData, int pixelSize, auto&& pointDataInserter) -> void;
  static [[nodiscard]] auto LoadAndCentreGeometry(std::ranges::input_range auto &&pixelData, cell_size pixelSize) -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto LoadGeometry(std::ranges::input_range auto &&pixelData, cell_size pixelSize, float shiftX, float shiftY) -> winrt::com_ptr<ID2D1Geometry>;

private:

  inline static level_geometries* m_instance { nullptr };

  inline static auto m_playerPixelImage = std::array {
    std::string { "    000    " },
    std::string { "    000    " },
    std::string { "    000    " },
    std::string { "  0000000  " },
    std::string { "  0000000  " },
    std::string { "  0000000  " },
    std::string { "00000000000" },
    std::string { "00000000000" },
    std::string { "00000000000" }
  };

  inline static auto m_playerThrustPixelImage = std::array {
    std::string { "00000" },
    std::string { "00000" },
    std::string { " 000 " },
    std::string { " 000 " }
  };

  inline static auto m_enemyStalkerPixelImage = std::array {
    std::string { "  000000  " },
    std::string { "  000000  " },
    std::string { "0000000000" },
    std::string { "0000000000" },
    std::string { "0000000000" },
    std::string { "00  00  00" },
    std::string { "00  00  00" }
  };

  inline static auto m_enemyTurretPixelImage = std::array {
    std::string { "   0000   " },
    std::string { "   0000   " },
    std::string { "   0000   " },
    std::string { "0000000000" },
    std::string { "0000000000" },
    std::string { " 00000000 " },
    std::string { " 00000000 " },
    std::string { "0000000000" },
    std::string { "0000000000" }
  };

  inline static auto m_playerBulletPixelImage = std::array {
    std::string { "0" },
    std::string { "0" },
    std::string { "0" }
  };

  inline static auto m_enemyBulletPixelImage = std::array {
    std::string { "0" }
  };

  inline static auto m_powerupPixelImage = std::array {
    std::string { "  0  " },
    std::string { " 000 " },
    std::string { "00000" },
    std::string { " 000 " },
    std::string { "  0  " }
  };

  inline static auto m_portalPixelImage = std::array {
    std::string { "  00  " },
    std::string { " 0000 " },
    std::string { "000000" },
    std::string { " 0000 " },
    std::string { "  00  " }
  };

  using point_data_collection = std::vector<POINT_2F>;

  winrt::com_ptr<ID2D1Geometry> m_rectangleGeometry;
  std::vector<winrt::com_ptr<ID2D1Geometry>> m_hudTargetGeometries;

  winrt::com_ptr<ID2D1Geometry> m_player;
  point_data_collection m_playerThrustData;
  winrt::com_ptr<ID2D1Geometry> m_playerThrust;
  winrt::com_ptr<ID2D1Geometry> m_playerBullet;
  winrt::com_ptr<ID2D1Geometry> m_enemy1;
  winrt::com_ptr<ID2D1Geometry> m_enemy2;
  winrt::com_ptr<ID2D1Geometry> m_enemy3;
  winrt::com_ptr<ID2D1Geometry> m_enemyBullet;
  winrt::com_ptr<ID2D1Geometry> m_portal;
  winrt::com_ptr<ID2D1Geometry> m_powerUp;
  std::map<int, winrt::com_ptr<ID2D1Geometry>> m_boundaryWallsLookup;

};

inline [[nodiscard]] auto level_geometries::get(const default_object& defaultObject) -> winrt::com_ptr<ID2D1Geometry>
{
  return defaultObject.Visit([](const auto& object) { return get(object); });
}

[[nodiscard]] auto level_geometries::get(auto&& object) -> winrt::com_ptr<ID2D1Geometry>
{
  return m_instance->Get(object);
}

inline auto level_geometries::getPlayerThrust() -> winrt::com_ptr<ID2D1Geometry>
{
  return m_instance->m_playerThrust;
}

auto level_geometries::Get(auto &&object) -> winrt::com_ptr<ID2D1Geometry>
{
  return m_rectangleGeometry;
}

inline [[nodiscard]] auto level_geometries::RectangleGeometry() -> winrt::com_ptr<ID2D1Geometry>
{
  return m_instance->m_rectangleGeometry;
}

inline [[nodiscard]] auto level_geometries::HudTargetGeometries() -> const std::vector<winrt::com_ptr<ID2D1Geometry>>&
{
  return m_instance->m_hudTargetGeometries;
}

// inline auto level_geometries::LoadAndCentreGeometryData(auto pixelDataBegin, auto pixelDataEnd, int pixelSize, auto &&pointDataInserter) -> void
inline auto level_geometries::LoadAndCentreGeometryData(std::ranges::input_range auto&& pixelData, int pixelSize, auto &&pointDataInserter) -> void
{
  std::vector<std::tuple<int, int, char>> pixelValues;
  std::copy_if(std::begin(pixelData), std::end(pixelData), std::back_inserter(pixelValues), [](auto&& pixelValue) -> bool
  {
    auto&& [column, row, value] = pixelValue;
    return value == '0';
  });
  
  std::vector<POINT_2F> orderedPoints;
  point_data::CellsToBoundary(pixelValues, pixelSize, pixelSize, std::back_inserter(orderedPoints));

  auto geometryBounds = point_data::GetBounds(orderedPoints);
  auto shiftLeft = ( geometryBounds.left + geometryBounds.right ) / 2.0f;
  auto shiftUp = ( geometryBounds.bottom + geometryBounds.top ) / 2.0f;

  std::ranges::transform(orderedPoints, pointDataInserter, [shiftLeft,shiftUp](auto&& pointData) -> POINT_2F { return { pointData.x - shiftLeft, pointData.y - shiftUp }; });
}

inline auto level_geometries::LoadAndCentreGeometry(std::ranges::input_range auto &&pixelData, cell_size pixelSize) -> winrt::com_ptr<ID2D1Geometry>
{
  std::vector<POINT_2I> pointData;
  pixel_geometry_loader::imageDataToOrderedPointData(pixelData, pixelSize, std::back_inserter(pointData), [](auto&& pixelData) -> bool { return pixelData.value == '0'; });
  auto pointDataAsFloat = std::ranges::views::transform(pointData, [](auto&& pointData) { return ToFloat(pointData); });
  auto geometryBounds = pixel_geometry_loader::getGeometryBounds(pointDataAsFloat);
  auto shiftLeft = ( geometryBounds.left + geometryBounds.right ) / 2.0f;
  auto shiftUp = ( geometryBounds.bottom + geometryBounds.top ) / 2.0f;
  auto shiftedPointData = std::ranges::views::transform(pointData, [shiftLeft,shiftUp](auto&& pointData) -> POINT_2F { return { pointData.x - shiftLeft, pointData.y - shiftUp }; });
  return direct2d::CreatePathGeometry(d2d_factory::get_raw(), shiftedPointData, D2D1_FIGURE_END_CLOSED);
}

auto level_geometries::LoadGeometry(std::ranges::input_range auto &&pixelData, cell_size pixelSize, float shiftX, float shiftY) -> winrt::com_ptr<ID2D1Geometry>
{
  std::vector<POINT_2I> pointData;
  pixel_geometry_loader::imageDataToOrderedPointData(pixelData, pixelSize, std::back_inserter(pointData), [](auto&& pixelData) -> bool { return pixelData.value == '0'; });
  auto pointDataAsFloat = std::ranges::views::transform(pointData, [](auto&& pointData) { return ToFloat(pointData); });
  auto shiftedPointData = std::ranges::views::transform(pointDataAsFloat, [shiftX, shiftY](auto&& pointData) -> POINT_2F { return { pointData.x + shiftX, pointData.y + shiftY }; });
  return direct2d::CreatePathGeometry(d2d_factory::get_raw(), shiftedPointData, D2D1_FIGURE_END_CLOSED);
}
