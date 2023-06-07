#pragma once

#include "object_input_data.h"
#include "geometry.h"

class explosion
{
public:

  class particle_state
  {

  public:

    particle_state();
    auto Initialize(float x, float y) -> void;
    auto Update(float interval) -> void;
    auto GetRenderRect(ID2D1SolidColorBrush* brush) const -> render_rect;
    [[nodiscard]] auto HasExpired() -> bool;

  private:

    float m_startX { 0 };
    float m_startY { 0 };
    float m_x = 0;
    float m_y = 0;
    float m_xVelocity = 0;
    float m_yVelocity = 0;
  };

  using particle_collection = std::list<particle_state>;
  
  explosion(float x, float y);

  [[nodiscard]] auto Particles() const -> const particle_collection&;
  
  auto Update(int64_t tickCount) -> void;

  [[nodiscard]] auto Destroyed() const -> bool;

private:

  particle_collection m_particles;
};
