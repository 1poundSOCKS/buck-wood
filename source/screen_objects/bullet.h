#ifndef _bullet_
#define _bullet_

#include "play_event.h"
#include "collision_data.h"
#include "collision_effect.h"
#include "object_input_data.h"

class bullet
{
public:

  bullet(float x, float y, float angle);

  auto Update(const object_input_data& inputData, int64_t tickCount) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto GetCollisionData() const -> const collision_data&;
  [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool;
  [[nodiscard]] auto GetCollisionEffect() const -> collision_effect;
  auto ApplyCollisionEffect(const collision_effect& effect) -> void;
  [[nodiscard]] auto Destroyed() const -> bool;

  [[nodiscard]] auto HasExpired() -> bool;

private:

  auto Initialize(ID2D1RenderTarget* renderTarget) -> void;

  float startX = 0, startY = 0;
  float xPos = 0, yPos = 0;
  float xVelocity = 0, yVelocity = 0;
  float angle = 0;
  float range = 1000;
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  collision_data m_collisionData;
  collision_effect m_collisionEffect;
  bool destroyed = false;
};

#endif
