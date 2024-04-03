#include "pch.h"
#include "enemy_type_1.h"

enemy_type_1::enemy_type_1(POINT_2F position) : base_object { position, { 1, 1 }, 0 }
{
}

auto enemy_type_1::Update(float interval, POINT_2F target) -> void
{
  MoveTowards(300 * interval, target);
}

[[nodiscard]] auto enemy_type_1::CanShootAt(POINT_2F position) const -> bool
{
  return false;
}

[[nodiscard]] auto enemy_type_1::Reloaded() const -> bool
{
  return false;
}

[[nodiscard]] auto enemy_type_1::Health() const -> float
{
  return 1;
}

auto enemy_type_1::ApplyDamage(int value) -> void
{

}
