#pragma once

#include "performance_counter.h"
#include "player_ship.h"
#include "level_target.h"
#include "explosion.h"
#include "renderers.h"
#include "asteroid_container.h"
#include "mine.h"
#include "explosion_particle.h"
#include "level_input.h"
#include "solid_objects.h"
#include "blank_objects.h"

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

  using bullet_collection = std::list<bullet>;
  using target_collection = std::vector<level_target>;
  using mine_collection = std::vector<mine>;
  using explosion_particle_collection  = std::list<explosion_particle>;
  using impact_particle_collection  = std::list<impact_particle>;

  level_container();
  level_container(const level_container& levelContainer) = delete;

  auto SetTimeout(int time) -> void;
  auto HasTimedOut() const -> bool;

  auto SetCentre(const game_point& value) -> void;
  [[nodiscard]] auto Centre() const -> const game_point&;

  auto AddSolidObjects(std::ranges::input_range auto&& objects) -> void;
  auto AddBlankObjects(std::ranges::input_range auto&& objects) -> void;
  auto AddTargets(std::ranges::input_range auto&& cells) -> void;
  auto AddMines(std::ranges::input_range auto&& cells) -> void;

  auto Update(const level_input& input, int64_t ticks, D2D1_RECT_F viewRect) -> void;
  auto Render(D2D1_RECT_F viewRect) const -> void;

  [[nodiscard]] auto Targets() const -> const target_collection&;
  [[nodiscard]] auto PlayerPosition() const -> game_point;
  [[nodiscard]] auto PlayerAngle() const -> float;
  [[nodiscard]] auto PlayerHasThrusterOn() const -> bool;
  [[nodiscard]] auto PlayerDied() const -> bool;
  [[nodiscard]] auto PlayerShields() const -> const player_ship::shield_status&;
  [[nodiscard]] auto TicksRemaining() const -> int64_t;
  [[nodiscard]] auto IsComplete() const -> bool;
  [[nodiscard]] auto HasFinished() const -> bool;
  [[nodiscard]] auto UpdateEvents() const -> const update_events&;

private:

  inline static const auto m_shotTimeNumerator { 1 };
  inline static const auto m_shotTimeDenominator { 20 };

  auto DoCollisions() -> void;
  auto CreateExplosion(const game_point& position) -> void;

  player_ship m_playerShip;
  reload_timer m_reloadTimer;

  bullet_collection m_bullets;
  target_collection m_targets;
  mine_collection m_mines;
  // asteroid_container m_asteroids;
  solid_objects m_solidObjects;
  blank_objects m_blankObjects;
  explosion_particle_collection m_explosionParticles;
  impact_particle_collection m_impactParticles;
  game_point m_centre;

  int64_t m_ticksRemaining = 0;

  bool m_playerHasThrusterOn = false;
  int m_activatedTargetCount = 0;

  update_events m_updateEvents;
};

inline auto level_container::update_events::reset() -> void
{
  playerShot = false ;
  targetActivated = false;
  mineExploded = false;
}

inline auto level_container::SetCentre(const game_point& value) -> void
{
  m_centre = value;
}

[[nodiscard]] inline auto level_container::Centre() const -> const game_point&
{
  return m_centre;
}

auto level_container::AddSolidObjects(std::ranges::input_range auto&& objects) -> void
{
  std::ranges::for_each(objects, [this](const auto& object)
  {
    m_solidObjects.push_back(object);
  });
}

auto level_container::AddBlankObjects(std::ranges::input_range auto&& objects) -> void
{
  std::ranges::for_each(objects, [this](const auto& object)
  {
    m_blankObjects.push_back(object);
  });
}

auto level_container::AddTargets(std::ranges::input_range auto&& cells) -> void
{
  std::ranges::for_each(cells, [this](const auto& cell)
  {
    m_targets.emplace_back( level_target { cell.Position().x, cell.Position().y } );
  });
}

auto level_container::AddMines(std::ranges::input_range auto&& positions) -> void
{
  std::ranges::for_each(positions, [this](const auto& position)
  {
    m_mines.emplace_back( mine { static_cast<float>(position.x), static_cast<float>(position.y) } );
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
