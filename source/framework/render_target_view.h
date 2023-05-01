#pragma once

#include "object_input_data.h"
#include "screen_input_state.h"

class render_target_view
{

public:

  render_target_view();

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto SetTransform(const D2D1::Matrix3x2F& transform) -> bool;
  [[nodiscard]] auto GetTransform() const -> const D2D1::Matrix3x2F&;
  [[nodiscard]] auto GetViewRect() const -> D2D1_RECT_F;
  [[nodiscard]] auto GetObjectInputData(const screen_input_state& screenInputState) -> object_input_data;

private:

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  D2D1::Matrix3x2F m_transform;
  D2D1::Matrix3x2F m_invertedTransform;
};
