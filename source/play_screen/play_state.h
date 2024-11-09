#pragma once

#include "game_level_data_loader.h"
#include "level_container.h"
#include "play_events.h"
#include "game_score.h"

class play_state
{

public:

  play_state();

  auto Update(float interval) -> void;
  auto SaveGameState() noexcept -> void;
  auto Reset() noexcept -> void;

  [[nodiscard]] auto LevelIndex() const noexcept -> int;
  [[nodiscard]] auto IncrementLevelIndex() noexcept -> int;
  [[nodiscard]] auto TimedOut() const noexcept -> bool;

  [[nodiscard]] auto LevelOver(const level_container& levelContainer) const noexcept -> bool;
  [[nodiscard]] auto LevelComplete(const level_container& levelContainer) const noexcept -> bool;
  [[nodiscard]] auto GameOver(const level_container& levelContainer) const noexcept -> bool;
  [[nodiscard]] auto GameComplete(const level_container& levelContainer) const noexcept -> bool;

  [[nodiscard]] auto Score() const -> const game_score&;
  [[nodiscard]] auto Score() -> game_score&;
  [[nodiscard]] auto PowerUpCount(const level_container& levelContainer) const noexcept -> std::size_t;

private:

  int m_levelIndex { 0 };
  game_score m_score;
  constexpr static float m_levelTimeLimit { 30.0f };
  float m_timeRemaining { m_levelTimeLimit };

};
