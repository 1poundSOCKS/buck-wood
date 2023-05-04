#include "pch.h"
#include "level_transform_transition.h"
#include "screen_view.h"

level_transform_transition::level_transform_transition(float startX, float startY, float startZoom, float endX, float endY, float endZoom) :
  m_startX(startX), m_startY(startY), m_startZoom(startZoom), m_endX(endX), m_endY(endY), m_endZoom(endZoom)
{
  
}

auto level_transform_transition::Get(float renderTargetWidth, float renderTargetHeight, int64_t totalTicks, int64_t currentTicks) const -> D2D1::Matrix3x2F
{
  auto cx = m_endX - m_startX;
  auto cy = m_endY - m_startY;
  auto cZoom = m_endZoom - m_startZoom;

  // auto elapsedTicks = totalTicks - currentTicks;
  auto x = cx * currentTicks / totalTicks + m_startX;
  auto y = cy * currentTicks / totalTicks + m_startY;
  auto zoom = cZoom * currentTicks / totalTicks + m_startZoom;

  return CreateGameLevelTransform(x, y, zoom, renderTargetWidth, renderTargetHeight);
}
