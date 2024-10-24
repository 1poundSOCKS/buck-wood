#include "pch.h"
#include "play_state.h"
#include "save_data.h"
#include "player_state.h"

play_state::play_state() : 
  m_score { std::make_shared<game_score>(game_score::value_type::total) }, 
  m_levelIndex { game_state::level_index() }, 
  m_levelContainer { std::make_shared<level_container>() },
  m_playerState { m_levelContainer->PlayerState() },
  m_lastPlayerState { m_playerState ? *m_playerState : m_lastPlayerState }
{
  m_score->Set(game_state::score());
}

auto play_state::LoadCurrentLevel() -> bool
{
  m_levelContainer = std::make_shared<level_container>();

  if( game_level_data_loader::loadLevel(m_levelIndex, *m_levelContainer) )
  {
    log::write(log::type::info, "Load current level successful: index={}", m_levelIndex);
    m_playerState = m_levelContainer->PlayerState();
    m_lastPlayerState = m_playerState ? *m_playerState : m_lastPlayerState;
    return true;
  }
  else
  {
    return false;
  }
}

auto play_state::LoadNextLevel() -> bool
{
  if( game_level_data_loader::testLoadLevel(m_levelIndex + 1) )
  {
    m_levelContainer = std::make_shared<level_container>();
    game_level_data_loader::loadLevel(++m_levelIndex, *m_levelContainer);
    game_state::set_level_index(m_levelIndex);
    log::write(log::type::info, "Load next level successful: index={}", m_levelIndex);
    m_playerState = m_levelContainer->PlayerState();
    m_lastPlayerState = m_playerState ? *m_playerState : m_lastPlayerState;
    return true;
  }
  else
  {
    log::write(log::type::info, "Load next level failed: index={}", m_levelIndex);
    return false;
  }
}

auto play_state::Update(float interval, RECT_F view) -> void
{
  game_level_data_loader::updateLevel(game_state::level_index(), m_levelContainer.get(), interval);
  m_levelContainer->Update(interval, view, m_lastPlayerState, LevelComplete());
  m_playerState = m_levelContainer->PlayerState();
  m_lastPlayerState = m_playerState ? *m_playerState : m_lastPlayerState;
  m_score->Add(play_events::get(play_events::counter_type::enemies_destroyed) * 50);
  m_score->Add(play_events::get(play_events::counter_type::bullets_destroyed) * 20);
  player_state::add_missiles(play_events::get(play_events::counter_type::power_ups_collected));
}

auto play_state::SaveGameState() noexcept -> void
{
  game_state::set_score(m_score->Value());
  game_state::set_power_up_count(player_state::missile_count());
  save_data::write(game_state::get());
}

auto play_state::LevelOver() const noexcept -> bool
{
  return !m_playerState || PowerUpCount() == 0;
}

auto play_state::LevelComplete() const noexcept -> bool
{
  return m_playerState && PowerUpCount() == 0;
}

auto play_state::GameOver() const noexcept -> bool
{
  return !m_playerState;
}

auto play_state::GameComplete() const noexcept -> bool
{
  return LevelOver() && game_level_data_loader::testLoadLevel(m_levelIndex + 1);
}

[[nodiscard]] auto play_state::LevelContainer() const -> const level_container&
{
  return *m_levelContainer;
}

[[nodiscard]] auto play_state::LevelContainer() -> level_container&
{
  return *m_levelContainer;
}

[[nodiscard]] auto play_state::Score() const -> const game_score&
{
  return *m_score;
}

[[nodiscard]] auto play_state::Score() -> game_score&
{
  return *m_score;
}

auto play_state::LastPlayerState() const noexcept -> player_ship_state
{
  return m_lastPlayerState;
}

[[nodiscard]] auto play_state::PowerUpCount() const noexcept -> std::size_t
{
  return std::accumulate(std::begin(m_levelContainer->Objects()), std::end(m_levelContainer->Objects()), static_cast<std::size_t>(0), [](std::size_t count, auto&& defaultObject)
  {
    return defaultObject.HoldsAlternative<power_up>() ? count + 1 : count;
  });
}
