#pragma once

#include "framework.h"
#include "play_scene.h"
#include "play_state.h"
#include "main_play_scene.h"
#include "closing_play_scene.h"

class play_screen
{
public:

  play_screen();
  auto Refresh(int64_t ticks) -> bool;

private:

  auto Update(int64_t ticks) -> bool;
  auto NextScene() -> bool;
  auto StartNextLevel() -> bool;
  auto Render() -> void;
  auto RenderDiagnostics() -> void;
  [[nodiscard]] auto PausePressed() -> bool;
  auto TogglePause() noexcept -> void;
  [[nodiscard]] auto Paused() const noexcept -> bool;

private:

  play_menu_controller m_menuController;
  std::shared_ptr<play_state> m_playState;
  bool m_playerDestroyed { false };

  enum class scene_type { none, main, closing };
  scene_type m_currentSceneType { scene_type::main };

  // main_play_scene m_mainPlayScene;
  // closing_play_scene m_closingPlayScene;
  std::unique_ptr<play_scene> m_currentScene;
  bool m_paused { false };

};
