#pragma once

#include "collision_data.h"
#include "collision_effect.h"
#include "object_input_data.h"
#include "path_geometry.h"

class level_asteroid
{
public:

  level_asteroid(const game_closed_object& object);

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;
  auto Update(const object_input_data& inputData, int64_t tickCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto GetCollisionData() -> collision_data;
  [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool;
  [[nodiscard]] auto GetCollisionEffect() const -> collision_effect;
  auto ApplyCollisionEffect(const collision_effect& effect) -> void;
  [[nodiscard]] auto Destroyed() const -> bool;

private:

  game_closed_object m_object;
  collision_effect m_collisionEffect;
  collision_data m_collisionData;
  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> m_fillBrush;
  winrt::com_ptr<ID2D1SolidColorBrush> m_borderBrush;
  path_geometry m_geometry;
};
