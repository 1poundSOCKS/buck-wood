#ifndef _bullet_
#define _bullet_

#include "play_event.h"
#include "collision_data.h"
#include "collision_effect.h"

class bullet
{
public:

  bullet(float x, float y, float angle);

  auto Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void;
  auto Update(int64_t tickCount, play_event_inserter playEventInserter) -> void;
  [[nodiscard]] auto LevelIsComplete() const -> bool;
  auto Render(D2D1_RECT_F viewRect) const -> void;
  [[nodiscard]] auto GetCollisionData() -> collision_data;
  [[nodiscard]] auto HasCollidedWith(const collision_data& collisionData) const -> bool;
  [[nodiscard]] auto GetCollisionEffect() const -> collision_effect;
  auto ApplyCollisionEffect(const collision_effect& effect, play_event_inserter playEventInserter) -> void;
  [[nodiscard]] auto Destroyed() const -> bool;

  [[nodiscard]] auto HasExpired() -> bool;

private:

  float startX = 0, startY = 0;
  float xPos = 0, yPos = 0;
  float xVelocity = 0, yVelocity = 0;
  float angle = 0;
  float range = 1000;  
  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<ID2D1SolidColorBrush> brush;
  collision_effect m_collisionEffect;
  collision_data m_collisionData;
  bool destroyed = false;
};

#endif
