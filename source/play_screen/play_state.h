#pragma once

#include "play_events.h"
#include "game_score.h"

class play_state
{

public:

  [[nodiscard]] auto Events() const -> const play_events&
  {
    return m_events;
  }

  [[nodiscard]] auto Events() -> play_events&
  {
    return m_events;
  }

  [[nodiscard]] auto Score() const -> const game_score&
  {
    return m_score;
  }

  [[nodiscard]] auto Score() -> game_score&
  {
    return m_score;
  }

private:

  play_events m_events;
  game_score m_score;

};
