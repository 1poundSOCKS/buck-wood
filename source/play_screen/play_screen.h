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

  auto Update(int64_t ticks) -> bool;
  auto Render() -> void;
  auto RenderUI() -> void;
  auto RenderDiagnostics() -> void;

  [[nodiscard]] auto PausePressed() -> bool;
  [[nodiscard]] auto Paused() const -> bool;

  auto Pause() -> void;
  auto Resume() -> void;

  auto InitializeScenes() -> bool;

private:

  bool m_paused = false;
  bool m_continueRunning = true;
  scene_controller m_sceneController;
  play_menu_controller m_menuController;
  game_level_data_loader m_gameLevelDataLoader;

};
