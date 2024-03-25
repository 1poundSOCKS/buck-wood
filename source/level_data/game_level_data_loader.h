#pragma once

#include "level_container.h"
#include "play_events.h"
#include "demo_level.h"
#include "game_clock.h"
#include "random_velocity.h"

class level_update_event
{

public:

  level_update_event(float wait, std::function<void(level_container*)> event) : m_timer { wait }, m_event { event }
  {
  }

  auto Update(float interval, level_container* levelContainer) -> bool
  {
    bool complete = m_timer.Update(interval);

    if( complete )
    {
      m_event(levelContainer);
    }

    return complete;
  }

private:

  reload_timer m_timer;
  std::function<void(level_container*)> m_event;

};

class game_level_data_loader
{

public:

  enum class state_type { starting, active, finished };

  game_level_data_loader() = default;

  auto LoadLevel(auto&&...args) -> std::unique_ptr<level_container>;
  auto UpdateLevel(level_container* levelContainer, float interval) -> void;

  [[nodiscard]] auto MoreLevels() const -> bool;
  [[nodiscard]] auto NextLevel() -> bool;
  [[nodiscard]] auto CurrentLevel() const -> int;
  [[nodiscard]] auto MoreUpdates() const -> bool;

  auto CreatePlayer(level_container* levelContainer) -> void;
  auto CreateTarget(level_container* levelContainer) -> void;

private:

  int m_levelIndex { -1 };
  inline static int m_levelCount { 99 };
  random_velocity m_randomVelocity { 100, 300 };

  std::vector<level_update_event> m_events;
  std::vector<level_update_event>::iterator m_currentEvent;

  demo_level m_demoLevel;

};

auto game_level_data_loader::LoadLevel(auto&&...args) -> std::unique_ptr<level_container>
{
  std::unique_ptr<level_container> levelContainer = std::make_unique<level_container>(level_container::level_type::arena, m_levelIndex, m_demoLevel.BoundaryPoints(), m_demoLevel.PlayerPosition(), std::forward<decltype(args)>(args)...);

  levelContainer->CreatePortal(POINT_2F {0, 0});

  m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreatePlayer(levelContainer); });
  m_events.emplace_back(3.0f, [this](level_container* levelContainer) -> void { CreateTarget(levelContainer); });

  m_currentEvent = std::begin(m_events);

  return levelContainer;
}

inline auto game_level_data_loader::UpdateLevel(level_container* levelContainer, float interval) -> void
{
  m_currentEvent = m_currentEvent != std::end(m_events) && m_currentEvent->Update(interval, levelContainer) ? std::next(m_currentEvent) : m_currentEvent;
}

inline [[nodiscard]] auto game_level_data_loader::MoreLevels() const -> bool
{
  return m_levelIndex + 1 < m_levelCount;
}

inline [[nodiscard]] auto game_level_data_loader::NextLevel() -> bool
{
  return ++m_levelIndex < m_levelCount;
}

inline [[nodiscard]] auto game_level_data_loader::CurrentLevel() const -> int
{
  return m_levelIndex;
}

inline [[nodiscard]] auto game_level_data_loader::MoreUpdates() const -> bool
{
  return m_currentEvent != std::end(m_events);
}

inline auto game_level_data_loader::CreatePlayer(level_container* levelContainer) -> void
{
  levelContainer->CreatePlayer(m_demoLevel.PlayerPosition());
}

inline auto game_level_data_loader::CreateTarget(level_container* levelContainer) -> void
{
  levelContainer->CreateTarget(POINT_2F { 0, 0 }, 4.0f, 10);
}
