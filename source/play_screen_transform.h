#pragma once

#include "screen_input_state.h"
#include "object_input_data.h"

class play_screen_transform
{
public:
  
  play_screen_transform(float centreX, float centreY, float scale, D2D1_SIZE_F renderTargetSize);
  [[nodiscard]] auto Get() const -> D2D1::Matrix3x2F;
  [[nodiscard]] auto GetViewRect(D2D1_SIZE_F renderTargetSize) const -> D2D1_RECT_F;
  [[nodiscard]] auto GetObjectInputData(const screen_input_state& screenInputState) -> object_input_data;

private:

  D2D1::Matrix3x2F m_transform;

};
