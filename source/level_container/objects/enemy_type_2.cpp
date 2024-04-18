#include "pch.h"
#include "enemy_type_2.h"

enemy_type_2::enemy_type_2(POINT_2F position, int hitpoints, float speed, float reloadTime) : 
  enemy_object { position, { 1.5, 1.5 }, 0, hitpoints }, m_speed { speed }, m_reloadTimer { reloadTime }
{
}

auto enemy_type_2::Update(float interval) -> void
{
  base_object::Update(interval);

  m_destination = m_destination ? m_destination : NewDestination();
  m_destination = MoveTowards(m_speed * interval, *m_destination) ? NewDestination() : m_destination;
  m_reloaded = m_reloadTimer.Update(interval);
}

auto enemy_type_2::NewDestination() -> POINT_2F
{
  static std::uniform_int_distribution<int> directionDist { 0, 7 };
  auto direction = directionDist(pseudo_random_generator::get()) * 45;
  return direct2d::CalculatePosition(m_position, static_cast<float>(direction), 100);
}
