#pragma once

#include "collision_data.h"
#include "collision_effect.h"
#include "object_input_data.h"
#include "collision_data.h"
#include "geometry.h"

class explosion_state
{
public:

  class particle_state
  {

  public:

    particle_state();
    auto Initialize(float x, float y) -> void;
    auto Update(float interval) -> void;
    auto HasCollidedWith(const collision_data& collisionData) const -> bool;
    auto GetRenderRect(ID2D1SolidColorBrush* brush) const -> render_rect;

  private:

    float m_x = 0;
    float m_y = 0;
    float m_xVelocity = 0;
    float m_yVelocity = 0;
  };

  explosion_state(float x, float y);
  
  auto Update(const object_input_data& inputData, int64_t tickCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto GetCollisionData() const -> const collision_data&;
  [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool;
  [[nodiscard]] auto GetCollisionEffect() const -> collision_effect;
  auto ApplyCollisionEffect(const collision_effect& effect) -> void;
  [[nodiscard]] auto Destroyed() const -> bool;

private:

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;

  winrt::com_ptr<ID2D1SolidColorBrush> m_brush;
  using particle_collection = std::list<particle_state>;
  particle_collection m_particles;
  collision_data m_collisionData;
};
