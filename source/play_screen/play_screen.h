#pragma once

#include "framework.h"
#include "scene_controller.h"
#include "play_scene.h"
#include "play_events.h"

class play_screen
{
public:

  play_screen();
  auto Refresh(int64_t ticks) -> bool;

private:

  auto Update(int64_t ticks) -> bool;
  auto Render() -> void;
  auto RenderDiagnostics() -> void;
  auto LoadNextLevel() -> bool;
  [[nodiscard]] auto PausePressed() -> bool;

private:

  scene_controller m_sceneController;
  play_menu_controller m_menuController;
  play_events m_playEvents;
  game_level_data_loader m_gameLevelDataLoader;

};
