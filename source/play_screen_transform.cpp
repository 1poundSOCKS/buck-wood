#include "pch.h"
#include "play_screen_transform.h"
#include "screen_transform.h"

play_screen_transform::play_screen_transform(float centreX, float centreY, float scale, D2D1_SIZE_F renderTargetSize)
{
  m_transform = D2D1::Matrix3x2F::Translation(-centreX, -centreY) * D2D1::Matrix3x2F::Scale(scale, scale) * D2D1::Matrix3x2F::Translation(renderTargetSize.width / 2, renderTargetSize.height / 2);
}

[[nodiscard]] auto play_screen_transform::Get() const -> D2D1::Matrix3x2F
{
  return m_transform;  
}

[[nodiscard]] auto play_screen_transform::GetViewRect(D2D1_SIZE_F renderTargetSize) const -> D2D1_RECT_F
{
  screen_transform screenTransform(m_transform);
  return screenTransform.GetViewRect(renderTargetSize);
}

[[nodiscard]] auto play_screen_transform::GetObjectInputData(const screen_input_state& screenInputState) -> object_input_data
{
  screen_transform screenTransform(m_transform);
  return screenTransform.GetObjectInputData(screenInputState);
}
