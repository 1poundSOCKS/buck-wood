#pragma once

#include "game_level_data_loader.h"
#include "level_container.h"
#include "play_events.h"
#include "game_score.h"

class play_state
{

public:

  enum class status { running, end_of_level, end_of_game };

  play_state();

  auto LoadCurrentLevel() -> void;
  auto LoadNextLevel() -> bool;
  auto Update(float interval, RECT_F view) -> void;
  auto SaveGameState() noexcept -> void;

  [[nodiscard]] auto Status() const -> status;
  [[nodiscard]] auto LevelContainer() const -> const level_container&;
  [[nodiscard]] auto LevelContainer() -> level_container&;
  [[nodiscard]] auto Score() const -> const game_score&;
  [[nodiscard]] auto Score() -> game_score&;
  [[nodiscard]] auto DataLoader() const noexcept -> const game_level_data_loader&;

private:

  [[nodiscard]] auto CalculateStatus() const -> status;

private:

  status m_status { status::running };
  game_level_data_loader m_dataLoader;
  std::shared_ptr<game_score> m_score;
  std::shared_ptr<level_container> m_levelContainer;

};
