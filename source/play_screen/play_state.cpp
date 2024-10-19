#include "pch.h"
#include "play_state.h"
#include "save_data.h"
#include "player_state.h"

play_state::play_state() : 
  m_score { std::make_shared<game_score>(game_score::value_type::total) }, m_levelContainer { std::make_shared<level_container>() }
{
  m_score->Set(game_state::score());
}

auto play_state::LoadCurrentLevel() -> bool
{
  m_levelContainer = std::make_shared<level_container>();

  if( game_level_data_loader::loadLevel(game_state::level_index(), *m_levelContainer) )
  {
    return true;
  }
  else
  {
    return false;
  }
}

auto play_state::LoadNextLevel() -> bool
{
  ++m_levelIndex;
  game_state::set_level_index(m_levelIndex);

  auto levelContainer = std::make_shared<level_container>();
  
  if( game_level_data_loader::loadLevel(m_levelIndex, *levelContainer) )
  {
    m_levelContainer = levelContainer;
    return true;
  }
  else 
  {
    return false;
  }
}

auto play_state::Update(float interval, RECT_F view) -> void
{
  game_level_data_loader::updateLevel(game_state::level_index(), m_levelContainer.get(), interval);
  m_levelContainer->Update(interval, view);
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

auto play_state::GameOver() const noexcept -> bool
{
  return m_levelContainer->PlayerState().Destroyed();
}

auto play_state::LevelOver() const noexcept -> bool
{
  return m_levelContainer->PlayerState().Destroyed() || m_levelContainer->ObjectCount(level_container::object_type::power_up) == 0;
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

auto play_state::LevelCentrePoint() const noexcept -> POINT_2F
{
  return m_levelContainer->CentrePoint();
}
