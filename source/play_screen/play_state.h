#pragma once

#include "game_level_data_loader.h"
#include "level_container.h"
#include "play_events.h"
#include "game_score.h"

class play_state
{

public:

  play_state() : m_events { std::make_shared<play_events>() }, m_score { std::make_shared<game_score>() }, m_levelContainer { std::make_shared<level_container>(m_events, m_score) }
  {
  }

  auto LoadLevel() -> bool
  {
    if( m_dataLoader.NextLevel() )
    {
      m_levelContainer = m_dataLoader.LoadLevel(m_events, m_score);
      return true;
    }
    else
    {
      return false;
    }
  }

  auto Update() -> void
  {
    m_loadComplete = m_dataLoader.UpdateLevel(m_levelContainer.get()) ? false : true;
  }

  [[nodiscard]] auto LevelComplete() const -> bool
  {
    return m_loadComplete && m_levelContainer->IsComplete();
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

  [[nodiscard]] auto Events() const -> const play_events&
  {
    return *m_events;
  }

  [[nodiscard]] auto Events() -> play_events&
  {
    return *m_events;
  }

  [[nodiscard]] auto Score() const -> const game_score&
  {
    return *m_score;
  }

  [[nodiscard]] auto Score() -> game_score&
  {
    return *m_score;
  }

private:

  game_level_data_loader m_dataLoader;
  std::shared_ptr<play_events> m_events;
  std::shared_ptr<game_score> m_score;
  std::shared_ptr<level_container> m_levelContainer;
  bool m_loadComplete { false };

};
