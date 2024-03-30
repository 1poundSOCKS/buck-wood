#pragma once

#include "enemy_type.h"
#include "enemy_movement_random.h"

class enemy_type_2 : public base_object
{

public:

  enum class state_type { moving, shooting };

  enemy_type_2(POINT_2F position);
  enemy_type_2(POINT_2F position, float reloadTime, int hitpoints);

  auto Update(float interval) -> void;

  [[nodiscard]] auto State() const -> state_type;
  [[nodiscard]] auto Destination() const -> std::optional<POINT_2F>;
  [[nodiscard]] auto CanShootAt(POINT_2F position) const -> bool;
  [[nodiscard]] auto Reloaded() const -> bool;
  [[nodiscard]] auto Health() const -> float;

  auto SetDestination(std::optional<POINT_2F> destination) -> void;
  auto ApplyDamage(int value) -> void;

private:

  state_type m_state { state_type::moving };
  constexpr static int m_movesBeforeStateChange { 3 };
  constexpr static float m_speed = 500;
  inline static std::uniform_int_distribution<int> m_positionDist { -10, 10 };

  std::optional<POINT_2F> m_destination;
  int m_maxHitpoints { 5 };
  int m_hitpoints { 5 };
  reload_timer m_reloadTimer { 0.1f };
  bool m_reloaded { false };
  constexpr static int m_shotsBeforeStateChange { 3 };

  int m_actionCount { 0 };

};

inline enemy_type_2::enemy_type_2(POINT_2F position, float reloadTime, int hitpoints) : 
  base_object { position, { 1.5, 1.5 }, 0 }, m_reloadTimer { reloadTime }, m_hitpoints { hitpoints }
{
}

inline auto enemy_type_2::Update(float interval) -> void
{
  base_object::Update(interval);

  switch( m_state )
  {
    case state_type::moving:
    {
      m_destination = m_destination ? m_destination : std::optional<POINT_2F>({static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100, static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100});
      m_position = m_destination ? direct2d::MoveTowards(m_position, *m_destination, m_speed * interval) : m_position;
      bool atDestination = m_destination && direct2d::AreEqual(m_position, *m_destination);
      m_destination = atDestination ? std::optional<POINT_2F>({static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100, static_cast<float>(m_positionDist(pseudo_random_generator::get())) * 100})  : m_destination;
      m_actionCount += atDestination ? 1 : 0;
      bool stateChange = m_actionCount < m_movesBeforeStateChange ? false : true;
      m_state = stateChange ? state_type::shooting : m_state;
      m_actionCount = stateChange ? 0 : m_actionCount;
      break;
    }
    case state_type::shooting:
    {
      m_reloaded = m_reloadTimer.Update(interval);
      m_actionCount += m_reloaded ? 1 : 0;
      bool stateChange = m_actionCount < m_shotsBeforeStateChange ? false : true;
      m_state = stateChange ? state_type::moving : m_state;
      m_actionCount = stateChange ? 0 : m_actionCount;
      m_reloaded = stateChange ? false : m_reloaded;
      break;
    }
  }
}

inline [[nodiscard]] auto enemy_type_2::State() const -> state_type
{
  return m_state;
}

inline [[nodiscard]] auto enemy_type_2::Destination() const -> std::optional<POINT_2F>
{
  return m_destination;
}

inline [[nodiscard]] auto enemy_type_2::Health() const -> float
{
  return static_cast<float>(m_hitpoints) / static_cast<float>(m_maxHitpoints);
}

inline [[nodiscard]] auto enemy_type_2::CanShootAt(POINT_2F position) const -> bool
{
  return m_reloaded && direct2d::GetDistanceBetweenPoints(m_position, position) < 1500;
}

inline [[nodiscard]] auto enemy_type_2::Reloaded() const -> bool
{
  return m_reloaded;
}

inline auto enemy_type_2::SetDestination(std::optional<POINT_2F> destination) -> void
{
  m_destination = destination;
  m_actionCount += ( m_destination && destination && !direct2d::AreEqual(*m_destination, *destination) ) ? 1 : 0;
}

inline auto enemy_type_2::ApplyDamage(int value) -> void
{
  m_hitpoints = std::max(0, m_hitpoints - value);
  m_destroyed = m_hitpoints == 0;
}
