#pragma once

#include "framework.h"
#include "play_scene.h"
#include "play_state.h"

class play_screen
{
public:

  play_screen();
  auto Refresh(int64_t ticks) -> bool;

private:

  auto Update(int64_t ticks) -> bool;
  auto NextScene() -> bool;
  auto StartNextLevel() -> bool;
  auto RestartLevel() -> bool;
  auto Render() -> void;
  auto RenderDiagnostics() -> void;
  [[nodiscard]] auto PausePressed() -> bool;
  auto TogglePause() noexcept -> void;
  [[nodiscard]] auto Paused() const noexcept -> bool;

private:

  play_menu_controller m_menuController;
  std::shared_ptr<play_state> m_playState;
  bool m_playerDestroyed { false };

  enum class scene_type { none, opening, main, closing, final };
  scene_type m_currentSceneType;

  std::unique_ptr<play_scene> m_currentScene;
  bool m_paused { false };

};
