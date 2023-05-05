#pragma once

#include "object_input_data.h"
#include "screen_input_state.h"

class screen_transform
{

public:

  screen_transform();
  auto Set(const D2D1::Matrix3x2F& transform) -> bool;
  [[nodiscard]] auto Get() const -> const D2D1::Matrix3x2F&;
  [[nodiscard]] auto GetViewRect(ID2D1RenderTarget* renderTarget) const -> D2D1_RECT_F;
  [[nodiscard]] auto GetObjectInputData(const screen_input_state& screenInputState) -> object_input_data;

private:

  D2D1::Matrix3x2F m_transform;
  D2D1::Matrix3x2F m_invertedTransform;
};
