#pragma once

class level_transform_transition
{
public:

  level_transform_transition(float startX, float startY, float startZoom, float endX, float endY, float endZoom);

  auto Get(float renderTargetWidth, float renderTargetHeight, int64_t totalTicks, int64_t currentTicks) const -> D2D1::Matrix3x2F;

private:

  float m_startX = 0;
  float m_startY = 0;
  float m_startZoom = 0;
  float m_endX = 0;
  float m_endY = 0;
  float m_endZoom = 0;

};
