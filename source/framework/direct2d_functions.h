#pragma once

#include "directx_functions.h"

namespace direct2d
{
  struct VELOCITY_2F
  {
    float x;
    float y;
  };

  inline auto GetDistanceBetweenPoints(D2D1_POINT_2F point1, D2D1_POINT_2F point2) -> float
  {
    float cx = point2.x - point1.x;
    float cy = point2.y - point1.y;
    return sqrt( cx * cx + cy * cy );
  }

  inline auto GetRadiusFromBounds(D2D1_RECT_F bounds) -> float
  {
    float topLeft = GetDistanceBetweenPoints({0, 0}, {bounds.left, bounds.top});
    float topRight = GetDistanceBetweenPoints({0, 0}, {bounds.right, bounds.top});
    float bottomLeft = GetDistanceBetweenPoints({0, 0}, {bounds.left, bounds.bottom});
    float bottomRight = GetDistanceBetweenPoints({0, 0}, {bounds.right, bounds.bottom});
    return std::max({topLeft, topRight, bottomLeft, bottomRight});
  }

  inline auto GetGeometryRadius(ID2D1Geometry* geometry) -> float
  {
    D2D1_RECT_F bounds;
    return SUCCEEDED(geometry->GetBounds(NULL, &bounds)) ? GetRadiusFromBounds(bounds) : 0;
  }

  inline auto GetAngleBetween(D2D1_POINT_2F startPoint, D2D1_POINT_2F endPoint) -> float
  {
    float radians = atan2(endPoint.y - startPoint.y, endPoint.x - startPoint.x);
    float degrees = RADTODEG(radians);
    return degrees + 90.0f;
  }

  inline auto GetRectAtPosition(D2D1_POINT_2F position, float cx, float cy) -> D2D1_RECT_F
  {
    auto halfWidth = cx / 2;
    auto halfHeight = cy / 2;
    return { position.x - halfWidth, position.y - halfHeight, position.x + halfWidth, position.y + halfHeight };
  }

  inline auto ShiftPosition(D2D1_POINT_2F position, float cx, float cy) -> D2D1_POINT_2F
  {
    return { position.x + cx, position.y + cy  };
  }

  inline auto CalculateVelocity(float speed, float direction) -> VELOCITY_2F
  {
    return { speed * sin(DEGTORAD(direction)), -speed * cos(DEGTORAD(direction)) };
  }

  inline auto CombineVelocities(VELOCITY_2F velocity1, VELOCITY_2F velocity2) -> VELOCITY_2F
  {
    return { velocity1.x + velocity2.x, velocity1.y + velocity2.y };
  }
}
