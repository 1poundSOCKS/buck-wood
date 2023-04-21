#pragma once

#include "level_object_container.h"

class object_container_view
{

public:

  object_container_view();

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto SetTransform(const D2D1::Matrix3x2F& transform) -> bool;
  auto Update(level_object_container& objectContainer, const screen_input_state& inputState, int64_t elapsedTicks) -> void;
  auto Render(const level_object_container& objectContainer) const -> void;

private:

  [[nodiscard]] auto GetViewRect() const -> D2D1_RECT_F;

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  D2D1::Matrix3x2F m_transform;
  D2D1::Matrix3x2F m_invertedTransform;
};
