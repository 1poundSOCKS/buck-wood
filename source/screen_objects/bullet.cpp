#include "pch.h"
#include "bullet.h"
#include "game_constants.h"
#include "render_brush_defs.h"
#include "render_text_format_def.h"
#include "clock_frequency.h"

constexpr D2D1_RECT_F GetBulletRect()
{
  return { -4, -4, 4, 4 };
}

bullet::bullet(float x, float y, float angle) : startX(x), startY(y), xPos(x), yPos(y), angle(angle)
{
  static const float bulletSpeed = 300.0f;
  static const float bulletRange = 2000.0f;
  
  yVelocity = -bulletSpeed * cos(DEGTORAD(angle));
  xVelocity = bulletSpeed * sin(DEGTORAD(angle));

  m_collisionEffect.SetProperty(collision_effect::activates_target, true);
}

auto bullet::Initialize(ID2D1RenderTarget* renderTarget) -> void
{
  m_renderTarget.attach(renderTarget);
  m_renderTarget->AddRef();
  brush = screen_render_brush_yellow.CreateBrush(renderTarget);
}

auto bullet::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
  auto updateInterval = static_cast<float>(tickCount) / static_cast<float>(clock_frequency::get()) * gameSpeedMultiplier;
  xPos += ( xVelocity * updateInterval );
  yPos += ( yVelocity * updateInterval );

  destroyed = HasExpired();
}

auto bullet::Render(D2D1_RECT_F viewRect) const -> void
{
  const D2D1_RECT_F rect = GetBulletRect();
  m_renderTarget->FillRectangle(D2D1_RECT_F { rect.left + xPos, rect.top + yPos, rect.right + xPos, rect.bottom + yPos }, brush.get());
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
