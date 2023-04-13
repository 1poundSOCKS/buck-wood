#include "pch.h"
#include "bullet.h"
#include "game_constants.h"

constexpr D2D1_RECT_F GetBulletRect()
{
  return { -4, -4, 4, 4 };
}

bullet::bullet(float x, float y, float angle, screen_render_brush_selector brushes) : startX(x), startY(y), xPos(x), yPos(y), angle(angle)
{
  static const float bulletSpeed = 500.0f;
  static const float bulletRange = 2000.0f;
  
  yVelocity = -bulletSpeed * cos(DEGTORAD(angle));
  xVelocity = bulletSpeed * sin(DEGTORAD(angle));

  m_collisionEffect.SetProperty(collision_effect::activates_target, true);

  brush.attach(brushes[green]);
  brush->AddRef();
}

auto bullet::Update(int64_t tickFrequency, int64_t tickCount, play_event_inserter playEventInserter) -> void
{
  auto updateInterval = static_cast<float>(tickCount) / static_cast<float>(tickFrequency) * gameSpeedMultiplier;
  xPos += ( xVelocity * updateInterval );
  yPos += ( yVelocity * updateInterval );

  destroyed = HasExpired();
}

[[nodiscard]] auto bullet::LevelIsComplete() const -> bool
{
  return true;
}

auto bullet::RenderTo(ID2D1RenderTarget* renderTarget, D2D1_RECT_F viewRect) const -> void
{
  const D2D1_RECT_F rect = GetBulletRect();
  renderTarget->FillRectangle(D2D1_RECT_F { rect.left + xPos, rect.top + yPos, rect.right + xPos, rect.bottom + yPos }, brush.get());
}

[[nodiscard]] auto bullet::GetCollisionData() -> collision_data
{
  return { xPos, yPos };
}

[[nodiscard]] auto bullet::HasCollidedWith(const collision_data& collisionData) const -> bool
{
  return collisionData.PointInside(xPos, yPos);
}

[[nodiscard]] auto bullet::GetCollisionEffect() const -> collision_effect
{
  return m_collisionEffect;
}

auto bullet::ApplyCollisionEffect(const collision_effect& collisionEffect, play_event_inserter playEventInserter) -> void
{
  destroyed = collisionEffect.GetProperty(collision_effect::stops_bullets);
}

[[nodiscard]] auto bullet::Destroyed() const -> bool
{
  return destroyed;
}

[[nodiscard]] auto bullet::HasExpired() -> bool
{
  float cx = xPos - startX;
  float cy = yPos - startY;
  float distance = sqrt(cx * cx + cy * cy);
  return distance > range;
}
