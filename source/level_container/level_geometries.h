#pragma once

#include "framework.h"

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

  static [[nodiscard]] auto CreateAsteroidGeometry(float x, float y, float maxWidth, float maxHeight) -> winrt::com_ptr<ID2D1PathGeometry>
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

    return CreatePathGeometry(d2d_factory::get_raw(), irregularShape);
  }

  level_geometries() : 
    m_ductFanGeometry { CreatePathGeometry(d2d_factory::get_raw(), GetDuctFanGeometryData(200)) }, 
    m_playerShipGeometry { CreatePathGeometry(d2d_factory::get_raw(), GetPlayerGeometryData()) }
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

  static [[nodiscard]] auto PlayerShipGeometry() -> ID2D1PathGeometry*
  {
    return m_instance->m_playerShipGeometry.get();
  }

  static [[nodiscard]] auto MineGeometry() -> ID2D1PathGeometry*
  {
    return m_instance->m_mineGeometry.get();
  }

  static [[nodiscard]] auto TargetGeometry() -> ID2D1PathGeometry*
  {
    return m_instance->m_targetGeometry.get();
  }

  static [[nodiscard]] auto AsteroidGeometry() -> ID2D1PathGeometry*
  {
    return m_instance->m_asteroidGeometry.get();
  }

  static [[nodiscard]] auto DuctFanGeometry() -> ID2D1PathGeometry*
  {
    return m_instance->m_ductFanGeometry.get();
  }

private:

  winrt::com_ptr<ID2D1PathGeometry> m_playerShipGeometry;
  winrt::com_ptr<ID2D1PathGeometry> m_mineGeometry { CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 40, 40, 3 }) };
  winrt::com_ptr<ID2D1PathGeometry> m_targetGeometry { CreatePathGeometry(d2d_factory::get_raw(), shape_generator { 0, 0, 100, 100, 6 }) };
  winrt::com_ptr<ID2D1PathGeometry> m_asteroidGeometry { CreateAsteroidGeometry(0, 0, 200, 200) };
  winrt::com_ptr<ID2D1PathGeometry> m_ductFanGeometry;

};
