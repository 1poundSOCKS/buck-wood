#pragma once

using POINT_2F = D2D1_POINT_2F;
using RECT_F = D2D1_RECT_F;
using SIZE_F = D2D1_SIZE_F;

struct VELOCITY_2F
{
  float x;
  float y;
};

struct SCALE_2F
{
  float x;
  float y;
};

struct POINT_2I
{
  int32_t x;
  int32_t y;

  auto operator <(const POINT_2I& point) const -> bool
  {
    return x < point.x || ( x == point.x && y < point.y );
  }

  auto operator ==(const POINT_2I& point) const -> bool
  {
    return y == point.y && x == point.x;
  }
};

struct RECT_I
{
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;
};

struct LINE_2I
{
  POINT_2I start;
  POINT_2I end;
};

inline [[nodiscard]] auto ToFloat(POINT_2I value) -> POINT_2F
{
  return { static_cast<float>(value.x), static_cast<float>(value.y) };
}

inline [[nodiscard]] auto ToInt(POINT_2F value) -> POINT_2I
{
  return { static_cast<int>(value.x), static_cast<int>(value.y) };
}

inline [[nodiscard]] auto ToFloat(RECT_I value) -> RECT_F
{
  return { static_cast<float>(value.left), static_cast<float>(value.top), static_cast<float>(value.right), static_cast<float>(value.bottom) };
}

inline [[nodiscard]] auto ToInt(RECT_F value) -> RECT_I
{
  return { static_cast<int>(value.left), static_cast<int>(value.top), static_cast<int>(value.right), static_cast<int>(value.bottom) };
}

inline [[nodiscard]] auto IsEqual(POINT_2F point1, POINT_2F point2) -> bool
{
  return point1.x == point2.x && point1.y == point2.y;
}

inline [[nodiscard]] auto IsNotEqual(POINT_2F point1, POINT_2F point2) -> bool
{
  return point1.x != point2.x || point1.y != point2.y;
}
