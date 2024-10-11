#pragma once

#include "default_object.h"
#include "level_geometry_functions.h"
#include "level_base.h"
#include "pixel_geometry_loader.h"

class level_geometries
{

public:

  static auto create() -> void;
  static auto destroy() -> void;

  static [[nodiscard]] auto get(const default_object& defaultObject) -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto get(auto&& object) -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto RectangleGeometry() -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto HudTargetGeometries() -> const std::vector<winrt::com_ptr<ID2D1Geometry>>&;

private:

  level_geometries();

  [[nodiscard]] auto Get(const player_ship &object) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto Get(const player_bullet &object) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto Get(const enemy_ship &object) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto Get(const enemy_bullet &object) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto Get(const boundary_walls &object) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto Get(const boundary_exit &object) -> winrt::com_ptr<ID2D1Geometry>;
  [[nodiscard]] auto Get(auto&& object) -> winrt::com_ptr<ID2D1Geometry>;

  static [[nodiscard]] auto Scale(ID2D1Geometry* geometry, SIZE_F size) -> SCALE_2F;
  static [[nodiscard]] auto LoadHudTargetGeometries(auto&& geometryInserter) -> void;

  static [[nodiscard]] auto LoadPixelGeometry(std::ranges::input_range auto &&pixelData, cell_size pixelSize) -> winrt::com_ptr<ID2D1Geometry>;
  static [[nodiscard]] auto CreateBoundaryWallsGeometry(std::ranges::input_range auto &&pixelData, cell_size cellSize) -> winrt::com_ptr<ID2D1Geometry>;

private:

  inline static level_geometries* m_instance { nullptr };

  inline static auto m_playerPixelImage = std::array {
    std::string { "     00     " },
    std::string { "   000000   " },
    std::string { "  00000000  " },
    std::string { " 0000000000 " },
    std::string { " 0000000000 " },
    std::string { " 0000000000 " },
    std::string { "000      000" },
    std::string { "000      000" }
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
    std::string { "0000000000" },
    std::string { "0000000000" },
    std::string { "0000000000" },
    std::string { "0000000000" }
  };

  inline static auto m_playerBulletPixelImage = std::array {
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

 inline static std::array m_level0_data { 
    std::string { "XX       XXXXXX      XX             XXX" },
    std::string { "X       XX   XX                       X" },
    std::string { "X  1   XXX    X                        " },
    std::string { "      XXXX                             " },
    std::string { "                                      X" },
    std::string { "                                     XX" },
    std::string { "                         3      XXXXXXX" },
    std::string { "                                   XXXX" },
    std::string { "                                      X" },
    std::string { "XX            P                       X" },
    std::string { "XXXXXXXXXX                             " },
    std::string { "XXXXXXXXXXXXX                          " },
    std::string { "XXXXXXX                                " },
    std::string { "XXX                                  XX" },
    std::string { "                              XXXXXXXXX" },
    std::string { "         2                         XXXX" },
    std::string { "                                     XX" },
    std::string { "                                       " },
    std::string { "                                       " },
    std::string { "                                       " },
    std::string { "                                       " },
    std::string { "                3             2        " },
    std::string { "        X                              " },
    std::string { "XX      X                              " },
    std::string { "XXXX    X                1            X" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "X     XXX         XX         XXXXXXXXXX" }
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
  winrt::com_ptr<ID2D1Geometry> m_enemyBullet;
  winrt::com_ptr<ID2D1Geometry> m_portal;
  winrt::com_ptr<ID2D1Geometry> m_powerUp;
  std::vector<winrt::com_ptr<ID2D1Geometry>> m_boundaryWalls;
  winrt::com_ptr<ID2D1Geometry> m_boundaryLeftExit;
  winrt::com_ptr<ID2D1Geometry> m_boundaryTopExit;
  winrt::com_ptr<ID2D1Geometry> m_boundaryRightExit;
  winrt::com_ptr<ID2D1Geometry> m_boundaryBottomExit;

};

inline [[nodiscard]] auto level_geometries::get(const default_object& defaultObject) -> winrt::com_ptr<ID2D1Geometry>
{
  return defaultObject.Visit([](const auto& object) { return get(object); });
}

[[nodiscard]] auto level_geometries::get(auto&& object) -> winrt::com_ptr<ID2D1Geometry>
{
  return m_instance->Get(object);
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

auto level_geometries::LoadPixelGeometry(std::ranges::input_range auto&& pixelData, cell_size pixelSize) -> winrt::com_ptr<ID2D1Geometry>
{
  std::vector<POINT_2I> pointData;
  std::vector<POINT_2F> pointDataAsFloat;
  std::vector<POINT_2F> centredPointData;

  pixel_geometry_loader::imageDataToOrderedPointData(pixelData, pixelSize, std::back_inserter(pointData), [](auto&& pixelData) -> bool { return pixelData.value == '0'; });
  std::ranges::transform(pointData, std::back_inserter(pointDataAsFloat), [](auto&& pointData) { return ToFloat(pointData); });
  pixel_geometry_loader::centrePointData(pointDataAsFloat, std::back_inserter(centredPointData));
  return direct2d::CreatePathGeometry(d2d_factory::get_raw(), centredPointData, D2D1_FIGURE_END_CLOSED);
}

inline auto level_geometries::CreateBoundaryWallsGeometry(std::ranges::input_range auto &&levelData, cell_size cellSize) -> winrt::com_ptr<ID2D1Geometry>
{
  std::vector<POINT_2I> pointData;
  std::vector<POINT_2F> pointDataAsFloat;

  pixel_geometry_loader::imageDataToOrderedPointData(levelData, cellSize, std::back_inserter(pointData), [](auto&& pixelData) -> bool { return pixelData.value != 'X'; });
  std::ranges::transform(pointData, std::back_inserter(pointDataAsFloat), [](auto&& pointData) { return ToFloat(pointData); });
  return direct2d::CreatePathGeometry(d2d_factory::get_raw(), pointDataAsFloat, D2D1_FIGURE_END_CLOSED);
}
