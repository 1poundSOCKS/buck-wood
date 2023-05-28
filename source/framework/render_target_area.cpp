#include "pch.h"
#include "render_target_area.h"

render_target_area::contraint_centred::contraint_centred(float width, float height) : m_width(width), m_height(height)
{
}

[[nodiscard]] auto render_target_area::contraint_centred::GetLeftBorder(float width) const -> float
{
  return ( width - width * m_width ) / 2;
}

[[nodiscard]] auto render_target_area::contraint_centred::GetTopBorder(float height) const -> float
{
  return ( height - height * m_height ) / 2;
}

[[nodiscard]] auto render_target_area::contraint_centred::GetRightBorder(float width) const -> float
{
  return GetLeftBorder(width);
}

[[nodiscard]] auto render_target_area::contraint_centred::GetBottomBorder(float height) const -> float
{
  return GetTopBorder(height);
}

render_target_area::constraint_bottom_centre::constraint_bottom_centre(float width, float height) : m_width(width), m_height(height)
{
}

[[nodiscard]] auto render_target_area::constraint_bottom_centre::GetLeftBorder(float width) const -> float
{
  return ( width - width * m_width ) / 2;
}

[[nodiscard]] auto render_target_area::constraint_bottom_centre::GetTopBorder(float height) const -> float
{
  return height - height * m_height;
}

[[nodiscard]] auto render_target_area::constraint_bottom_centre::GetRightBorder(float width) const -> float
{
  return GetLeftBorder(width);
}

[[nodiscard]] auto render_target_area::constraint_bottom_centre::GetBottomBorder(float height) const -> float
{
  return 0;
}

render_target_area::contraint_bottom_right::contraint_bottom_right(float width, float height) : m_width(width), m_height(height)
{
}

[[nodiscard]] auto render_target_area::contraint_bottom_right::GetLeftBorder(float width) const -> float
{
  return width - width * m_width;
}

[[nodiscard]] auto render_target_area::contraint_bottom_right::GetTopBorder(float height) const -> float
{
  return height - height * m_height;
}

[[nodiscard]] auto render_target_area::contraint_bottom_right::GetRightBorder(float width) const -> float
{
  return 0;
}

[[nodiscard]] auto render_target_area::contraint_bottom_right::GetBottomBorder(float height) const -> float
{
  return 0;
}

render_target_area::render_target_area(D2D1_SIZE_F renderTargetSize) : m_rect { 0, 0, renderTargetSize.width - 1, renderTargetSize.height - 1 }
{
}

[[nodiscard]] auto render_target_area::GetRect() const -> D2D1_RECT_F
{
  return m_rect;
}
