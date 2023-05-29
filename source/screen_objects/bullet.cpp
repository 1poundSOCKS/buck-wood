#include "pch.h"
#include "bullet.h"
#include "render_brush_defs.h"
#include "render_text_format_def.h"
#include "clock_frequency.h"
#include "renderers.h"

bullet::bullet(float x, float y, float angle) : startX(x), startY(y), xPos(x), yPos(y), angle(angle)
{
  static const float bulletSpeed = 500.0f;
  static const float bulletRange = 2000.0f;
  
  yVelocity = -bulletSpeed * cos(DEGTORAD(angle));
  xVelocity = bulletSpeed * sin(DEGTORAD(angle));

  m_collisionEffect.SetProperty(collision_effect::activates_target, true);
}

[[nodiscard]] auto bullet::Position() const -> game_point
{
  return { xPos, yPos };
}

auto bullet::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
  auto updateInterval = framework::gameUpdateInterval(tickCount);
  
  xPos += ( xVelocity * updateInterval );
  yPos += ( yVelocity * updateInterval );

  m_collisionData = { xPos, yPos };

  destroyed = HasExpired();
}

[[nodiscard]] auto bullet::GetCollisionData() const -> const collision_data&
{
  return m_collisionData;
}

[[nodiscard]] auto bullet::HasCollidedWith(const collision_data& collisionData) const -> bool
{
  return collisionData.PointInside(xPos, yPos);
}

[[nodiscard]] auto bullet::GetCollisionEffect() const -> collision_effect
{
  return m_collisionEffect;
}

auto bullet::ApplyCollisionEffect(const collision_effect& collisionEffect) -> void
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
