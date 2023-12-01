#pragma once

#include "framework.h"
#include "play_screen_scene.h"

class play_screen
{
public:

  play_screen();

  auto Refresh(int64_t ticks) -> bool;

private:

  auto Update(int64_t ticks) -> void;
  auto Render() -> void;

  auto RefreshCurrentScene(int64_t ticks) -> void;
  auto LoadNextLevel() -> bool;

  [[nodiscard]] auto PausePressed() -> bool;
  [[nodiscard]] auto Paused() const -> bool;
  auto Pause() -> void;
  auto Unpause() -> void;
  auto Quit() -> void;

private:

  using play_scene_ptr = std::unique_ptr<play_scene>;
  using play_scene_collection = std::vector<play_scene_ptr>;

  bool m_paused = false;
  bool m_continueRunning = true;
  level_container_ptr m_levelContainer;  
  play_menu_controller m_menuController;
  game_level_data_loader m_gameLevelDataLoader;
  play_scene_collection m_scenes;
  play_scene_collection::iterator m_currentScene;

};
