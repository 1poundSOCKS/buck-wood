#include "pch.h"
#include "level_asteroid.h"
#include "render_brush_defs.h"
#include "perlin_simplex_noise.h"
#include "renderers.h"

level_asteroid::level_asteroid(const game_closed_object& object) : m_object(object), m_collisionData(object), m_geometry(object)
{
  m_collisionEffect.SetProperty(collision_effect::stops_bullets, true);
  m_collisionEffect.SetProperty(collision_effect::kills_player, true);
}

level_asteroid::level_asteroid(game_rect rect)
{
  m_collisionEffect.SetProperty(collision_effect::stops_bullets, true);
  m_collisionEffect.SetProperty(collision_effect::kills_player, true);

  auto width = ( rect.bottomRight.x - rect.topLeft.x ) / 2.0f;
  auto height = ( rect.bottomRight.y - rect.topLeft.y ) / 2.0f;

  auto centrePoint = rect.CentrePoint();
  auto x = centrePoint.x;
  auto y = centrePoint.y;

  m_object.Reserve(360 / 30);

  for( int angle = 0; angle < 360; angle += 30 )
  {
    auto angleInRadians = DEGTORAD(angle);

    auto cx = width * sin(angleInRadians);
    auto cy = height * cos(angleInRadians);

    auto noise = psn::GetNoise(static_cast<float>(x + cx), static_cast<float>(y + cy));
    noise = ( noise + 5.0f ) / 6.0f;

    m_object += {x + cx * noise, y + cy * noise};
  }

  m_object.Finalize();
  m_geometry.Load(m_object);
  m_collisionData = collision_data { m_object };
}

[[nodiscard]] auto level_asteroid::Geometry() const -> const path_geometry&
{
  return m_geometry;
}

auto level_asteroid::Update(const object_input_data& inputData, int64_t tickCount) -> void
{
}

auto level_asteroid::Render(D2D1_RECT_F) const -> void
{
  renderer::render(*this);
}

[[nodiscard]] auto level_asteroid::GetCollisionData() const -> const collision_data&
{
  return m_collisionData;
}

[[nodiscard]] auto level_asteroid::HasCollidedWith(const collision_data& collisionData) const -> bool
{
  return false;
}

[[nodiscard]] auto level_asteroid::GetCollisionEffect() const -> collision_effect
{
  return m_collisionEffect;
}

auto level_asteroid::ApplyCollisionEffect(const collision_effect& effect) -> void
{
}

[[nodiscard]] auto level_asteroid::Destroyed() const -> bool
{
  return false;
}
