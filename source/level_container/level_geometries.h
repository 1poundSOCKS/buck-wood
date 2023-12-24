#pragma once

#include "framework.h"
// #include "level_objects.h"

class level_geometries
{

private:

  inline static level_geometries* m_instance { nullptr };

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
      game_point { -0.5, -0.5 },
      game_point { 0.5, -0.5 },
      game_point { 0.5, 0.5 },
      game_point { -0.5, 0.5 }
    };

    return ductFanGeometryData | std::ranges::views::transform([size](const game_point& gamePoint) -> game_point
    {
      return { gamePoint.x * size, gamePoint.y * size  };
    });
  }

  static [[nodiscard]] auto CreateAsteroidGeometry(float x, float y, float maxWidth, float maxHeight) -> path_geometry
  {
    auto xRatioNoise = psn::GetNoise(x / 7, y / 7);
    auto yRatioNoise = psn::GetNoise(x / 13, y / 13);

    auto xRatio = ( xRatioNoise + 1 ) / 4 + 0.5f;
    auto yRatio = ( yRatioNoise + 1 ) / 4 + 0.5f;
    
    auto width = maxWidth * xRatio;
    auto height = maxHeight * yRatio;

    auto shapeGenerator = shape_generator { 0, 0, width, height, 10 };

    auto irregularShape = shapeGenerator | std::ranges::views::transform([x, y](auto point)
    {
      auto noise = 0.7f + ( psn::GetNoise(x + point.x, y + point.y) + 1 ) * 0.1f;
      return game_point { point.x * noise, point.y * noise };
    });

    return path_geometry { irregularShape };
  }

  level_geometries() : m_ductFanGeometry { GetDuctFanGeometryData(200) }, m_playerShipGeometry { GetPlayerGeometryData() }
  {
  }

public:

  static auto create() -> void
  {
    destroy();
    m_instance = new level_geometries();
  }

  static auto destroy() -> void
  {
    if( m_instance )
    {
      delete m_instance;
      m_instance = nullptr;
    }
  }

  static [[nodiscard]] auto PlayerShipGeometry() -> const path_geometry&
  {
    return m_instance->m_playerShipGeometry;
  }

  static [[nodiscard]] auto MineGeometry() -> const path_geometry&
  {
    return m_instance->m_mineGeometry;
  }

  static [[nodiscard]] auto TargetGeometry() -> const path_geometry&
  {
    return m_instance->m_targetGeometry;
  }

  static [[nodiscard]] auto AsteroidGeometry() -> const path_geometry&
  {
    return m_instance->m_asteroidGeometry;
  }

  static [[nodiscard]] auto DuctFanGeometry() -> const path_geometry&
  {
    return m_instance->m_ductFanGeometry;
  }

private:

  path_geometry m_playerShipGeometry;
  path_geometry m_mineGeometry { shape_generator { 0, 0, 40, 40, 3 } };
  path_geometry m_targetGeometry { shape_generator { 0, 0, 100, 100, 6 } };
  path_geometry m_asteroidGeometry { CreateAsteroidGeometry(0, 0, 200, 200) };
  path_geometry m_ductFanGeometry;

};
