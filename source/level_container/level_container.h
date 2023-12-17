#pragma once

#include "performance_counter.h"
#include "player_ship.h"
#include "level_target.h"
#include "explosion.h"
#include "renderers.h"
#include "asteroid_container.h"
#include "mine.h"
#include "explosion_particle.h"
#include "thrust_particle.h"
#include "level_input.h"
#include "solid_objects.h"
#include "blank_objects.h"
#include "duct_fan.h"
#include "dynamic_object.h"
#include "dynamic_object_collection.h"
#include "shape_generator.h"

class level_container
{
public:

  struct update_events
  {
    auto reset() -> void;

    bool playerShot { false };
    bool targetActivated { false };
    bool mineExploded { false };
  };

  using mine_object = dynamic_object<mine>;

  using bullet_collection = std::list<bullet>;
  using target_collection = std::vector<level_target>;
  using mine_collection = dynamic_object_collection<mine>;
  using asteroid_collection = std::vector<level_asteroid>;
  using duct_fan_collection = std::vector<duct_fan>;
  using explosion_particle_collection  = std::list<explosion_particle>;
  using impact_particle_collection  = std::list<impact_particle>;
  using thrust_particle_collection = std::list<thrust_particle>;

  level_container() = default;
  level_container(const level_container& levelContainer) = delete;

  auto AddBlankObjects(std::ranges::input_range auto&& objects) -> void;
  auto AddTargets(std::ranges::input_range auto&& positions) -> void;
  auto AddAsteroids(std::ranges::input_range auto&& asteroids) -> void;
  auto AddDuctFans(std::ranges::input_range auto&& asteroids) -> void;

  auto Update(const level_input& input, int64_t ticks, D2D1_RECT_F viewRect) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  [[nodiscard]] auto Targets() const -> const target_collection&;
  [[nodiscard]] auto PlayerPosition() const -> game_point;
  [[nodiscard]] auto PlayerAngle() const -> float;
  [[nodiscard]] auto PlayerHasThrusterOn() const -> bool;
  [[nodiscard]] auto PlayerDied() const -> bool;
  [[nodiscard]] auto PlayerShields() const -> const player_ship::shield_status&;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto HasFinished() const -> bool;
  [[nodiscard]] auto UpdateEvents() const -> const update_events&;

private:

  inline static constexpr auto m_shotTimeNumerator { 1 };
  inline static constexpr auto m_shotTimeDenominator { 20 };

  path_geometry m_mineGeometry { shape_generator { 0, 0, 40, 40, 3 } };

  auto UpdatePlayer(const level_input& input, float interval) -> void;

  auto DoCollisions() -> void;
  auto DoPlayerShipCollisions() -> void;
  auto DoMineCollisions() -> void;
  auto DoBulletCollisions() -> void;
  auto DoExplosionParticleCollisions() -> void;
  auto DoThrustParticleCollisions() -> void;
  auto DoBorderCollisions(const blank_object& border) -> void;

  auto CreateExplosion(const game_point& position) -> void;

  player_ship m_playerShip;
  
  reload_timer m_reloadTimer { static_cast<float>(1) / static_cast<float>(20) };
  reload_timer m_thrustEmmisionTimer { static_cast<float>(1) / static_cast<float>(10) };

  bullet_collection m_bullets;
  target_collection m_targets;
  mine_collection m_mines;
  asteroid_collection m_asteroids;
  blank_objects m_blankObjects;
  duct_fan_collection m_ductFans;
  explosion_particle_collection m_explosionParticles;
  impact_particle_collection m_impactParticles;
  thrust_particle_collection m_thrustParticles;
  int m_activatedTargetCount { 0 };
  update_events m_updateEvents;
};

inline auto level_container::update_events::reset() -> void
{
  playerShot = false ;
  targetActivated = false;
  mineExploded = false;
}

auto level_container::AddBlankObjects(std::ranges::input_range auto&& objects) -> void
{
  std::ranges::for_each(objects, [this](const auto& object)
  {
    m_blankObjects.push_back(object);
  });
}

auto level_container::AddTargets(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_targets.emplace_back(position.x, position.y);
  });
}

auto level_container::AddAsteroids(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_asteroids.emplace_back(position.x, position.y, 200.0f, 200.0f);
  });
}

auto level_container::AddDuctFans(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_ductFans.emplace_back(position.x, position.y, 300.0f, 30.0f);
  });
}

[[nodiscard]] inline auto level_container::PlayerAngle() const -> float
{
  return m_playerShip.Angle();
}

[[nodiscard]] inline auto level_container::PlayerShields() const -> const player_ship::shield_status&
{
  return m_playerShip.ShieldStatus();  
}

[[nodiscard]] inline auto level_container::UpdateEvents() const -> const update_events&
{
  return m_updateEvents;
}
