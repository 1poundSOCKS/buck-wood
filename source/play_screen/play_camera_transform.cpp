#include "pch.h"
#include "play_camera_transform.h"
#include "screen_transform.h"

play_camera_transform::play_camera_transform(float centreX, float centreY, float angle, float scale, D2D1_SIZE_F renderTargetSize)
{
  m_transform = D2D1::Matrix3x2F::Translation(-centreX, -centreY) * D2D1::Matrix3x2F::Rotation(angle) * D2D1::Matrix3x2F::Scale(scale, scale) * D2D1::Matrix3x2F::Translation(renderTargetSize.width / 2, renderTargetSize.height / 2);
}

[[nodiscard]] auto play_camera_transform::Get() const -> D2D1::Matrix3x2F
{
  return m_transform;  
}
