#pragma once

#include "game_level_data_loader.h"
#include "level_cell_collection.h"
#include "level_container.h"
#include "play_events.h"
#include "game_score.h"

class play_state
{

public:

  enum class status { running, end_of_level, end_of_game, exit_level };

  play_state();

  auto LoadCurrentLevel() -> bool;
  auto LoadNextLevel() -> bool;
  auto Update(float interval, RECT_F view) -> status;
  auto SaveGameState() noexcept -> void;

  [[nodiscard]] auto GameOver() const noexcept -> bool;
  [[nodiscard]] auto LevelOver() const noexcept -> bool;

  [[nodiscard]] auto Status() const -> status;
  [[nodiscard]] auto LevelContainer() const -> const level_container&;
  [[nodiscard]] auto LevelContainer() -> level_container&;
  [[nodiscard]] auto Score() const -> const game_score&;
  [[nodiscard]] auto Score() -> game_score&;

  [[nodiscard]] auto LevelCentrePoint() const noexcept -> POINT_2F;

private:

  [[nodiscard]] auto CalculateStatus() const -> status;

private:

  int m_levelIndex { 0 };
  status m_status { status::running };
  std::shared_ptr<game_score> m_score;
  std::shared_ptr<level_container> m_levelContainer;

};
