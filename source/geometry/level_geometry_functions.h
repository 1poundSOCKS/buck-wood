#pragma once

namespace level_geometry_functions
{
  inline constexpr [[nodiscard]] auto GetPlayerGeometryData()
  {
    constexpr float width { 7 };
    constexpr float height { 10 };

    return std::array {
      D2D1_POINT_2F { 0, -height },
      D2D1_POINT_2F { width, height },
      D2D1_POINT_2F { -width, height }
    };
  }

  inline constexpr [[nodiscard]] auto GetPlayerBulletGeometryData()
  {
    constexpr float width { 7 };
    constexpr float height { 20 };

    return std::array {
      D2D1_POINT_2F { -width, -height },
      D2D1_POINT_2F { width, -height },
      D2D1_POINT_2F { width, height },
      D2D1_POINT_2F { -width, height }
    };
  }

  inline auto [[nodiscard]] GetDuctFanGeometryData(float size)
  {
    static constexpr auto ductFanGeometryData = std::array {
      D2D1_POINT_2F { -0.5, -0.5 },
      D2D1_POINT_2F { 0.5, -0.5 },
      D2D1_POINT_2F { 0.5, 0.5 },
      D2D1_POINT_2F { -0.5, 0.5 }
    };

    return ductFanGeometryData | std::ranges::views::transform([size](D2D1_POINT_2F gamePoint) -> D2D1_POINT_2F
    {
      return { gamePoint.x * size, gamePoint.y * size  };
    });
  }

  inline constexpr auto [[nodiscard]] GetRectangleGeometryData()
  {
    return std::array {
      D2D1_POINT_2F { -0.5, -0.5 },
      D2D1_POINT_2F { 0.5, -0.5 },
      D2D1_POINT_2F { 0.5, 0.5 },
      D2D1_POINT_2F { -0.5, 0.5 }
    };
  }

  inline constexpr auto [[nodiscard]] GetBoundaryWallsGeometryData()
  {
    return std::array {
      D2D1_POINT_2F { -0.5, -0.5 },
      D2D1_POINT_2F { 0.5, -0.5 },
      D2D1_POINT_2F { 0.5, 0.5 },
      D2D1_POINT_2F { -0.5, 0.5 }
    };
  }

  inline [[nodiscard]] auto CreateAsteroidGeometry(float x, float y, float maxWidth, float maxHeight) -> winrt::com_ptr<ID2D1PathGeometry>
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
      return D2D1_POINT_2F { point.x * noise, point.y * noise };
    });

    return direct2d::CreatePathGeometry(d2d_factory::get_raw(), irregularShape, D2D1_FIGURE_END_CLOSED);
  }

  inline constexpr auto GetHudTargetTopLeftGeometryData()
  {
    return std::array {
      D2D1_POINT_2F { -0.5, -0.3 },
      D2D1_POINT_2F { -0.5, -0.5 },
      D2D1_POINT_2F { -0.3, -0.5 }
    };
  }

  inline auto GetPlayerShieldElipse() -> D2D1_ELLIPSE
  {
    return D2D1::Ellipse({ 0, 0 }, 50, 50);
  }

  inline auto GetCircle() -> D2D1_ELLIPSE
  {
    return D2D1::Ellipse({ 0, 0 }, 1, 1);
  }


}
