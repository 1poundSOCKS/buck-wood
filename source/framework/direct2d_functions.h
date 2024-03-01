#pragma once

#include "directx_functions.h"
#include "basic_data_types.h"

namespace direct2d
{
  [[nodiscard]] auto CreateD2DFactory() -> winrt::com_ptr<ID2D1Factory>;
  [[nodiscard]] auto CreateRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) -> winrt::com_ptr<ID2D1RenderTarget>;
  [[nodiscard]] auto CreatePathGeometry(ID2D1Factory* d2dFactory) -> winrt::com_ptr<ID2D1PathGeometry>;
  [[nodiscard]] auto CreateTransformedGeometry(ID2D1Factory* d2dFactory, ID2D1Geometry* geometry, const D2D1_MATRIX_3X2_F& transform) -> winrt::com_ptr<ID2D1TransformedGeometry>;
  [[nodiscard]] auto CreateScreenRenderBrush(ID2D1RenderTarget* renderTarget, D2D1::ColorF color) -> winrt::com_ptr<ID2D1SolidColorBrush>;

  auto LoadPathGeometry(ID2D1PathGeometry* geometry, std::ranges::input_range auto&& points, D2D1_FIGURE_END figureEnd) -> void
  {
    winrt::com_ptr<ID2D1GeometrySink> sink;
    geometry->Open(sink.put());
    auto begin = std::ranges::begin(points);

    sink->BeginFigure({ (*begin).x, (*begin).y }, D2D1_FIGURE_BEGIN_FILLED);

    for( auto next = std::ranges::next(begin); next != std::ranges::end(points); ++next )
    {
      sink->AddLine({ (*next).x, (*next).y });
    }

    sink->EndFigure(figureEnd);
    sink->Close();
  }

  [[nodiscard]] auto CreatePathGeometry(ID2D1Factory* d2dFactory, std::ranges::input_range auto&& points, D2D1_FIGURE_END figureEnd) -> winrt::com_ptr<ID2D1PathGeometry>
  {
    winrt::com_ptr<ID2D1PathGeometry> geometry { CreatePathGeometry(d2dFactory) };
    LoadPathGeometry(geometry.get(), points, figureEnd);
    return geometry;
  }

  inline [[nodiscard]] auto GetSourceGeometry(ID2D1TransformedGeometry* geometry) -> winrt::com_ptr<ID2D1Geometry>
  {
    winrt::com_ptr<ID2D1Geometry> sourceGeometry;
    geometry->GetSourceGeometry(sourceGeometry.put());
    return sourceGeometry;
  }

  inline [[nodiscard]] auto CreateEllipseGeometry(ID2D1Factory* d2dFactory, const D2D1_ELLIPSE& ellipse) -> winrt::com_ptr<ID2D1EllipseGeometry>
  {
    winrt::com_ptr<ID2D1EllipseGeometry> geometry;
    d2dFactory->CreateEllipseGeometry(ellipse, geometry.put());
    return geometry;
  }

  inline [[nodiscard]] auto GetDistanceBetweenPoints(D2D1_POINT_2F point1, D2D1_POINT_2F point2) -> float
  {
    float cx = point2.x - point1.x;
    float cy = point2.y - point1.y;
    return sqrt( cx * cx + cy * cy );
  }

  inline [[nodiscard]] auto GetRadiusFromBounds(D2D1_RECT_F bounds) -> float
  {
    float topLeft = GetDistanceBetweenPoints({0, 0}, {bounds.left, bounds.top});
    float topRight = GetDistanceBetweenPoints({0, 0}, {bounds.right, bounds.top});
    float bottomLeft = GetDistanceBetweenPoints({0, 0}, {bounds.left, bounds.bottom});
    float bottomRight = GetDistanceBetweenPoints({0, 0}, {bounds.right, bounds.bottom});
    return std::max({topLeft, topRight, bottomLeft, bottomRight});
  }

  inline [[nodiscard]] auto GetGeometryRadius(ID2D1Geometry* geometry) -> float
  {
    D2D1_RECT_F bounds;
    return SUCCEEDED(geometry->GetBounds(NULL, &bounds)) ? GetRadiusFromBounds(bounds) : 0;
  }

  inline [[nodiscard]] auto GetSizeFromBounds(D2D1_RECT_F bounds) -> D2D1_SIZE_F
  {
    return { bounds.right - bounds.left, bounds.bottom - bounds.top };
  }
  
  inline [[nodiscard]] auto GetGeometrySize(ID2D1Geometry* geometry) -> D2D1_SIZE_F
  {
    D2D1_RECT_F bounds;
    return SUCCEEDED(geometry->GetBounds(NULL, &bounds)) ? GetSizeFromBounds(bounds) : D2D1_SIZE_F { 0, 0 };
  }

  inline [[nodiscard]] auto GetAngleBetweenPoints(D2D1_POINT_2F startPoint, D2D1_POINT_2F endPoint) -> float
  {
    float radians = atan2(endPoint.y - startPoint.y, endPoint.x - startPoint.x);
    float degrees = RADTODEG(radians);
    return degrees + 90.0f;
  }

  inline [[nodiscard]] auto GetRectAtPosition(D2D1_POINT_2F position, float cx, float cy) -> D2D1_RECT_F
  {
    auto halfWidth = cx / 2;
    auto halfHeight = cy / 2;
    return { position.x - halfWidth, position.y - halfHeight, position.x + halfWidth, position.y + halfHeight };
  }

  inline [[nodiscard]] auto ShiftPosition(D2D1_POINT_2F position, float cx, float cy) -> D2D1_POINT_2F
  {
    return { position.x + cx, position.y + cy  };
  }

  inline [[nodiscard]] auto ShiftPosition(D2D1_POINT_2F position, D2D1_POINT_2F value) -> D2D1_POINT_2F
  {
    return { position.x + value.x, position.y + value.y  };
  }

  inline [[nodiscard]] auto CalculateVelocity(float speed, float direction) -> VELOCITY_2F
  {
    return { speed * sin(DEGTORAD(direction)), -speed * cos(DEGTORAD(direction)) };
  }

  inline [[nodiscard]] auto CalculatePosition(float distance, float direction) -> D2D1_POINT_2F
  {
    return { distance * sin(DEGTORAD(direction)), -distance * cos(DEGTORAD(direction)) };
  }

  inline [[nodiscard]] auto CalculatePosition(D2D1_POINT_2F position, VELOCITY_2F velocity, float interval) -> D2D1_POINT_2F
  {
    return { position.x + velocity.x * interval, position.y + velocity.y * interval };
  }

  inline [[nodiscard]] auto CombineVelocities(VELOCITY_2F velocity1, VELOCITY_2F velocity2) -> VELOCITY_2F
  {
    return { velocity1.x + velocity2.x, velocity1.y + velocity2.y };
  }

  inline [[nodiscard]] auto MultiplyVelocity(VELOCITY_2F velocity, float amount) -> VELOCITY_2F
  {
    return { velocity.x * amount, velocity.y * amount };
  }

  inline [[nodiscard]] auto CalculateSpeed(VELOCITY_2F velocity) -> float
  {
    return sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
  }

  inline [[nodiscard]] auto CalculateDirection(VELOCITY_2F velocity) -> float
  {
    return direct2d::GetAngleBetweenPoints({0, 0}, {velocity.x, velocity.y});
  }

  inline [[nodiscard]] auto GetAngleDifference(float referenceAngle, float bearing) -> float
  {
    auto angleDifference = bearing - referenceAngle;
    angleDifference = angleDifference > 180 ? angleDifference - 360 : angleDifference;
    return angleDifference < -180 ? angleDifference + 360 : angleDifference;
  }

  inline [[nodiscard]] auto RotateAngle(float angle, float rotation) -> float
  {
    angle += rotation;
    
    while( angle < 0 )
    {
      angle += 360;
    }
    
    while( angle >= 360 )
    {
      angle -= 360;
    }

    return angle;
  }

  inline [[nodiscard]] auto CalculatePosition(D2D1_POINT_2F position, float angle, float distance) -> D2D1_POINT_2F
  {
    return ShiftPosition(position, CalculatePosition(distance, angle));
  }

  inline [[nodiscard]] auto Rotate(POINT_2F position, POINT_2F rotateAround, float angle) -> POINT_2F
  {
    auto distance = GetDistanceBetweenPoints(position, rotateAround);
    auto direction = RotateAngle(GetAngleBetweenPoints(rotateAround, position), angle);
    return CalculatePosition(rotateAround, direction, distance);
  }

  inline [[nodiscard]] auto MoveTowards(POINT_2F position, POINT_2F destination, float distance) -> POINT_2F
  {
    return GetDistanceBetweenPoints(position, destination) > distance ? CalculatePosition(position, GetAngleBetweenPoints(position, destination), distance) : destination;
  }

  inline [[nodiscard]] auto AreEqual(POINT_2F point1, POINT_2F point2) -> bool
  {
    return point1.x == point2.x && point1.y == point2.y;
  }
}
