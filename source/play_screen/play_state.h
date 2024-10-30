#pragma once

#include "game_level_data_loader.h"
#include "level_container.h"
#include "play_events.h"
#include "game_score.h"

class play_state
{

public:

  play_state();

  auto LoadCurrentLevel() -> bool;
  auto LoadNextLevel() -> bool;
  auto Update(float interval, RECT_F view) -> void;
  auto SaveGameState() noexcept -> void;

  [[nodiscard]] auto LevelOver() const noexcept -> bool;
  [[nodiscard]] auto LevelComplete() const noexcept -> bool;
  [[nodiscard]] auto GameOver() const noexcept -> bool;
  [[nodiscard]] auto GameComplete() const noexcept -> bool;

  [[nodiscard]] auto LevelContainer() const -> const level_container&;
  [[nodiscard]] auto LevelContainer() -> level_container&;
  [[nodiscard]] auto Score() const -> const game_score&;
  [[nodiscard]] auto Score() -> game_score&;
  [[nodiscard]] auto LastPlayerState() const noexcept -> player_ship_state;
  [[nodiscard]] auto PowerUpCount() const noexcept -> std::size_t;

private:

  auto VisitObject(enemy_ship& object) const noexcept -> void;
  auto VisitObject(auto&& object) const noexcept -> void;

private:

  int m_levelIndex { 0 };
  std::shared_ptr<game_score> m_score;
  std::shared_ptr<level_container> m_levelContainer;
  std::set<cell_id> m_emptyCellLookup;
  std::optional<player_ship_state> m_playerState;
  player_ship_state m_lastPlayerState;
  std::optional<cell_id> m_playerCell;
};

auto play_state::VisitObject(auto &&object) const noexcept -> void
{
}
