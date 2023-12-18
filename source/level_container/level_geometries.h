#pragma once

#include "framework.h"
#include "duct_fan.h"

class level_geometries
{

private:

  static constexpr auto GetPlayerGeometryData()
  {
    constexpr float width { 7 };
    constexpr float height { 10 };

    return std::array {
      game_point { 0, -height },
      game_point { width, height },
      game_point { -width, height }
    };
  }

  static [[nodiscard]] auto GetDuctFanGeometryData(float size)
  {
    static constexpr auto ductFanGeometryData = std::array {
      game_point { -0.05, -0.5 },
      game_point { 0.05, -0.5 },
      game_point { 0.05, 0.5 },
      game_point { -0.05, 0.5 }
    };

    return ductFanGeometryData | std::ranges::views::transform([size](const game_point& gamePoint) -> game_point
    {
      return { gamePoint.x * size, gamePoint.y * size  };
    });
  }

public:

  level_geometries() : m_ductFanGeometry { GetDuctFanGeometryData(300) }, m_playerShipGeometry { GetPlayerGeometryData() }
  {
  }

  [[nodiscard]] auto PlayerShipGeometry() const -> const path_geometry&
  {
    return m_playerShipGeometry;
  }

  [[nodiscard]] auto MineGeometry() const -> const path_geometry&
  {
    return m_mineGeometry;
  }

  [[nodiscard]] auto TargetGeometry() const -> const path_geometry&
  {
    return m_targetGeometry;
  }

  [[nodiscard]] auto DuctFanGeometry() const -> const path_geometry&
  {
    return m_ductFanGeometry;
  }

private:

  path_geometry m_playerShipGeometry;
  path_geometry m_mineGeometry { shape_generator { 0, 0, 40, 40, 3 } };
  path_geometry m_targetGeometry { shape_generator { 0, 0, 100, 100, 6 } };
  path_geometry m_ductFanGeometry;

};
