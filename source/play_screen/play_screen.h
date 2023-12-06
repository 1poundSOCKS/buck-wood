#pragma once

#include "framework.h"
#include "scene_controller.h"
#include "play_scene.h"

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
  scene_controller m_sceneController;
  play_scene::level_container_ptr m_levelContainer;
  play_menu_controller m_menuController;
  game_level_data_loader m_gameLevelDataLoader;

};
