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

inline [[nodiscard]] auto ToRectF(RECT_I rect) -> RECT_F
{
  return { static_cast<float>(rect.left), static_cast<float>(rect.top), static_cast<float>(rect.right), static_cast<float>(rect.bottom) };
}
