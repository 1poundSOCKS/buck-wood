#include "pch.h"
#include "bullet.h"

const float gameSpeedMultiplier = 2.0f;

constexpr D2D1_RECT_F GetBulletRect()
{
  return { -4, -4, 4, 4 };
}

bullet::bullet(screen_render_brush_selector brushes)
{
  brush.attach(brushes[green]);
  brush->AddRef();
}

auto bullet::Update(int64_t tickFrequency, int64_t tickCount, play_event_inserter playEventInserter) -> void
{
  auto updateInterval = static_cast<float>(tickCount) / static_cast<float>(tickFrequency) * gameSpeedMultiplier;
  xPos += ( xVelocity * updateInterval );
  yPos += ( yVelocity * updateInterval );
}

[[nodiscard]] auto bullet::HasCollided(float x, float y) const -> bool
{
  return false;
}

auto bullet::HitByBullet() -> void
{
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

[[nodiscard]] auto bullet::GetOutline() -> object_outline
{
  return outline;
}
