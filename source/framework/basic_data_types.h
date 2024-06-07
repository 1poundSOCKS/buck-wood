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
};

struct RECT_I
{
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;
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
