#ifndef _play_screen_
#define _play_screen_

#include "framework.h"
#include "game_objects.h"
#include "level_object_container.h"
#include "timers.h"
#include "sound_data.h"
#include "player_ship.h"
#include "bullet.h"
#include "screen_input_state.h"
#include "level_control_state.h"
#include "area_grid.h"
#include "level_target.h"
#include "level_island.h"
#include "sound_data.h"
#include "game_level_data.h"
#include "level_timer.h"
#include "level_state.h"
#include "mouse_cursor.h"
#include "diagnostics.h"
#include "game_level_data_loader.h"

class play_screen
{
public:

  play_screen();

  auto Initialize(ID2D1RenderTarget* renderTarget, IDWriteFactory* dwriteFactory) -> void;
  auto Update(const screen_input_state& inputState) -> void;
  auto Render() const -> void;
  auto PlaySoundEffects() const -> void;
  [[nodiscard]] auto ContinueRunning() const -> bool;
  auto FormatDiagnostics(diagnostics_data_inserter_type diagnosticsDataInserter) const -> void;

private:

  static level_control_state GetLevelControlState(const screen_input_state& inputState);

  auto UpdateMouseCursorPosition() -> void;
  auto UpdateLevelState(const screen_input_state& inputState) -> void;
  [[nodiscard]] auto CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, float renderScale = 1.0) -> D2D1::Matrix3x2F;
  auto PlaySoundEffects(const global_sound_buffer_selector& soundBuffers) const -> void;
  [[nodiscard]] auto GetMouseDiagnostics() const -> std::wstring;
  
  void OnGamePaused(const screen_input_state& inputState);
  void OnGameRunning(const screen_input_state& inputState);
  void OnGamePlaying(const screen_input_state& inputState);
  bool AllLevelsAreComplete();
  bool ScreenTransitionTimeHasExpired();
  void SetScreenTransitionDelay(int timeInSeconds);

  [[nodiscard]] auto LoadFirstLevel() -> bool;
  [[nodiscard]] auto LoadNextLevel() -> bool;
  auto LoadCurrentLevel() -> void;
  auto LoadPlayer() -> void;

  winrt::com_ptr<ID2D1RenderTarget> m_renderTarget;
  winrt::com_ptr<IDWriteFactory> m_dwriteFactory;

  performance_counter::data timer = { 0, 0, 0 };
  
  int64_t pauseStart = 0;
  int64_t pauseTotal = 0;
  int64_t levelStart = 0;
  int64_t transitionEnd = 0;

  bool m_gameComplete = false;
  bool continueRunning = true;

  game_level_data_index::const_iterator currentLevelDataIterator;
  game_level_data_index::const_iterator endLevelDataIterator;

  render_target_mouse_data renderTargetMouseData;
  game_rect m_viewRect;
  D2D1::Matrix3x2F m_viewTransform;

  level_object_container m_levelObjectContainer;
  int64_t levelTimeLimit;
  std::vector<float> levelTimes;

  game_level_data_loader m_gameLevelDataLoader;
  mouse_cursor m_mouseCursor;
  std::unique_ptr<player_ship> player;
  std::unique_ptr<level_timer> m_levelTimer;
  std::unique_ptr<level_state> m_levelState;

  float mouseX = 0;
  float mouseY = 0;

  bool playerShot = false;
  bool targetShot = false;
};

#endif
