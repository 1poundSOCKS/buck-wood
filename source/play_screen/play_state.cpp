#include "pch.h"
#include "play_state.h"
#include "save_data.h"
#include "player_state.h"

play_state::play_state() : 
  m_score { std::make_shared<game_score>(game_score::value_type::total) }, m_levelContainer { std::make_shared<level_container>() }
{
  m_score->Set(game_state::score());
}

auto play_state::LoadCurrentLevel() -> void
{
  m_levelContainer = game_level_data_loader::loadLevel();
  player_state::set_status(player_state::status::active);
}

auto play_state::LoadNextLevel() -> bool
{
  if( game_level_data_loader::nextLevel() )
  {
    m_levelContainer = game_level_data_loader::loadLevel();
    player_state::set_status(player_state::status::active);
    return true;
  }
  else
  {
    return false;
  }
}

auto play_state::Update(float interval, RECT_F view) -> void
{
  if( m_status == status::running )
  {
    game_level_data_loader::updateLevel(m_levelContainer.get(), interval);
  }

  m_levelContainer->Update(interval, view);

  m_status = CalculateStatus();

  switch( m_status )
  {
    case status::running:
      player_state::set_status(player_state::status::active);
      break;
    case status::end_of_level:
    case status::end_of_game:
      player_state::set_status(player_state::status::celebrating);
      break;
  }

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

auto play_state::Status() const -> status
{
  return m_status;
}

[[nodiscard]] auto play_state::CalculateStatus() const -> status
{
  if( m_levelContainer->PlayerDestroyed() )
  {
    return status::end_of_game;
  }
  
  if( game_level_data_loader::levelCanBeCompleted() && m_levelContainer->EnemyCount() == 0 )
  {
    return game_level_data_loader::moreLevels() ? status::end_of_level : status::end_of_game;
  }

  return status::running;
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

// [[nodiscard]] auto play_state::DataLoader() const noexcept -> const game_level_data_loader&
// {
//   return m_dataLoader;
// }
