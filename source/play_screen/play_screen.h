#pragma once

#include "framework.h"
#include "scene_controller.h"
#include "play_scene.h"
#include "play_events.h"
#include "game_score.h"

class play_screen
{
public:

  play_screen();
  auto Refresh(int64_t ticks) -> bool;

private:

  auto Update(int64_t ticks) -> bool;
  auto Render() -> void;
  auto RenderDiagnostics() -> void;
  auto LoadNextLevel(std::shared_ptr<level_container> currentLevelContainer) -> std::shared_ptr<level_container>;
  [[nodiscard]] auto PausePressed() -> bool;

private:

  scene_controller m_sceneController;
  play_menu_controller m_menuController;
  std::shared_ptr<play_events> m_playEvents;
  std::shared_ptr<game_score> m_gameScore;
  game_level_data_loader m_gameLevelDataLoader;
  std::shared_ptr<level_container> m_levelContainer;
  bool m_playerDestroyed { false };

};
