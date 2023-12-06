#pragma once

#include "framework.h"
#include "play_scene_controller.h"
#include "opening_play_scene.h"
#include "main_play_scene.h"
#include "closing_play_scene.h"

class play_screen
{
public:

  play_screen();

  auto Refresh(int64_t ticks) -> bool;

private:

  auto Update(int64_t ticks) -> void;
  auto Render() -> void;

  auto LoadNextLevel() -> bool;

  [[nodiscard]] auto PausePressed() -> bool;
  [[nodiscard]] auto Paused() const -> bool;

  auto Pause() -> void;
  auto Resume() -> void;
  auto Quit() -> void;

private:

  bool m_paused = false;
  bool m_continueRunning = true;
  level_container_ptr m_levelContainer;  
  play_scene_controller m_sceneController;
  play_menu_controller m_menuController;
  game_level_data_loader m_gameLevelDataLoader;

};
