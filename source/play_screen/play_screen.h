#pragma once

#include "framework.h"
#include "play_scene.h"
#include "play_state.h"
#include "main_play_scene.h"

class play_screen
{
public:

  play_screen();
  auto Refresh(int64_t ticks) -> bool;

private:

  auto Update(int64_t ticks) -> bool;
  auto Render() -> void;
  auto RenderDiagnostics() -> void;
  [[nodiscard]] auto PausePressed() -> bool;
  auto TogglePause() noexcept -> void;
  [[nodiscard]] auto Paused() const noexcept -> bool;

private:

  play_menu_controller m_menuController;
  std::shared_ptr<play_state> m_playState;
  bool m_playerDestroyed { false };

  main_play_scene m_mainPlayScene;
  bool m_paused { false };

};
