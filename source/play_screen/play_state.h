#pragma once

#include "game_level_data_loader.h"
#include "level_container.h"
#include "play_events.h"
#include "game_score.h"

class play_state
{

public:

  play_state() : 
    m_score { std::make_shared<game_score>(game_score::value_type::total) }, m_powerUpsCollected { std::make_shared<int>(0) },
    m_levelContainer { std::make_shared<level_container>() }
  {
  }

  auto LoadLevel() -> bool
  {
    if( m_dataLoader.NextLevel() )
    {
      m_levelContainer = m_dataLoader.LoadLevel();
      return true;
    }
    else
    {
      return false;
    }
  }

  auto Update(float interval, RECT_F view) -> void
  {
    m_dataLoader.UpdateLevel(m_levelContainer.get(), interval);
    m_levelContainer->Update(interval, view);

    m_score->Add(play_events::get(play_events::counter_type::enemies_destroyed) * 50);
    m_score->Add(play_events::get(play_events::counter_type::bullets_destroyed) * 20);
    *m_powerUpsCollected += play_events::get(play_events::counter_type::power_ups_collected);
  }

  [[nodiscard]] auto LevelComplete() const -> bool
  {
    return m_levelContainer->PlayerState().Destroyed() || (m_levelContainer->TargetCount() == 0 && !m_dataLoader.MoreUpdates());
  }

  [[nodiscard]] auto Complete() const -> bool
  {
    return m_levelContainer->PlayerState().Destroyed() || ( LevelComplete() && !m_dataLoader.MoreLevels() );
  }

  [[nodiscard]] auto LevelContainer() const -> const level_container&
  {
    return *m_levelContainer;
  }

  [[nodiscard]] auto LevelContainer() -> level_container&
  {
    return *m_levelContainer;
  }

  [[nodiscard]] auto Score() const -> const game_score&
  {
    return *m_score;
  }

  [[nodiscard]] auto Score() -> game_score&
  {
    return *m_score;
  }

  [[nodiscard]] auto PowerUps() const -> game_score
  {
    game_score powerUps(game_score::value_type::power_ups);
    powerUps.Add(*m_powerUpsCollected);
    return powerUps;
  }

private:

  game_level_data_loader m_dataLoader;
  std::shared_ptr<game_score> m_score;
  std::shared_ptr<int> m_powerUpsCollected;
  std::shared_ptr<level_container> m_levelContainer;

};
