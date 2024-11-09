#include "pch.h"
#include "play_state.h"
#include "save_data.h"
#include "player_state.h"

play_state::play_state() : 
  m_score { game_score::value_type::total }, 
  m_levelIndex { game_state::level_index() }
{
  m_score.Set(game_state::score());
}

auto play_state::Update(float interval) -> void
{
  m_timeRemaining -= interval;
  m_score.Add(play_events::get(play_events::counter_type::enemies_destroyed) * 50);
  player_state::add_missiles(play_events::get(play_events::counter_type::power_ups_collected));
}

auto play_state::SaveGameState() noexcept -> void
{
  game_state::set_score(m_score.Value());
  game_state::set_power_up_count(player_state::missile_count());
  save_data::write(game_state::get());
}

auto play_state::Reset() noexcept -> void
{
  m_timeRemaining = m_levelTimeLimit;
}

auto play_state::LevelIndex() const noexcept -> int
{
  return m_levelIndex;
}

auto play_state::IncrementLevelIndex() noexcept -> int
{
  return ++m_levelIndex;
}

auto play_state::TimedOut() const noexcept -> bool
{
  return m_timeRemaining < 0.0f;
}

auto play_state::LevelOver(const level_container& levelContainer) const noexcept -> bool
{
  return levelContainer.PlayerState().Destroyed() || PowerUpCount(levelContainer) == 0;
}

auto play_state::LevelComplete(const level_container& levelContainer) const noexcept -> bool
{
  return !levelContainer.PlayerState().Destroyed() && PowerUpCount(levelContainer) == 0;
}

auto play_state::GameOver(const level_container& levelContainer) const noexcept -> bool
{
  return levelContainer.PlayerState().Destroyed();
}

auto play_state::GameComplete(const level_container& levelContainer) const noexcept -> bool
{
  return LevelOver(levelContainer) && game_level_data_loader::testLoadLevel(m_levelIndex + 1);
}

[[nodiscard]] auto play_state::Score() const -> const game_score&
{
  return m_score;
}

[[nodiscard]] auto play_state::Score() -> game_score&
{
  return m_score;
}

[[nodiscard]] auto play_state::PowerUpCount(const level_container& levelContainer) const noexcept -> std::size_t
{
  return std::accumulate(std::begin(levelContainer.Objects()), std::end(levelContainer.Objects()), static_cast<std::size_t>(0), [](std::size_t count, auto&& defaultObject)
  {
    return defaultObject.HoldsAlternative<power_up>() ? count + 1 : count;
  });
}
