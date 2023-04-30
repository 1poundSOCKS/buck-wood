#pragma once

// #include "active_object_container.h"
// #include "passive_object_container.h"
#include "object_input_data.h"
#include "screen_input_state.h"

class object_container_view
{

public:

  object_container_view();

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto SetTransform(const D2D1::Matrix3x2F& transform) -> bool;
  [[nodiscard]] auto GetTransform() const -> const D2D1::Matrix3x2F&;
  // auto Update(active_object_container& objectContainer, const screen_input_state& inputState, int64_t elapsedTicks) -> void;
  // auto Update(passive_object_container& objectContainer, const screen_input_state& inputState, int64_t elapsedTicks) -> void;
  // auto Render(const active_object_container& objectContainer) const -> void;
  // auto Render(const passive_object_container& objectContainer) const -> void;
  [[nodiscard]] auto GetViewRect() const -> D2D1_RECT_F;
  [[nodiscard]] auto GetObjectInputData(const screen_input_state& screenInputState) -> object_input_data;

private:

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  D2D1::Matrix3x2F m_transform;
  D2D1::Matrix3x2F m_invertedTransform;
};
