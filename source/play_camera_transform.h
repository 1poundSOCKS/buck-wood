#pragma once

#include "screen_input_state.h"
#include "object_input_data.h"

class play_camera_transform
{
public:
  
  play_camera_transform(float centreX, float centreY, float angle, float scale, D2D1_SIZE_F renderTargetSize);
  [[nodiscard]] auto Get() const -> D2D1::Matrix3x2F;

private:

  D2D1::Matrix3x2F m_transform;

};
