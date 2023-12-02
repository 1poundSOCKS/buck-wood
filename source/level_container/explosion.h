#pragma once

#include "framework.h"

class explosion
{
public:

  class particle_state
  {

  public:

    particle_state();

    auto Initialize(float x, float y) -> void;
    auto Update(float interval) -> void;
    auto GetRenderRect() const -> D2D1_RECT_F;

    [[nodiscard]] auto DistanceTravelled() const -> float;
    [[nodiscard]] auto Range() const -> float;
    [[nodiscard]] auto HasExpired() const -> bool;

  private:

    static inline const float m_range { 500 };

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
  
  auto Update(float interval) -> void;

  [[nodiscard]] auto Destroyed() const -> bool;

private:

  particle_collection m_particles;
};
