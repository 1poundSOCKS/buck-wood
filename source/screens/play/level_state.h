#ifndef _level_state_
#define _level_state_

#include "game_level_data.h"
#include "player_ship.h"
#include "bullet.h"
#include "screen_input_state.h"
#include "screen_render_data.h"
#include "level_control_state.h"
#include "area_grid.h"
#include "explosion.h"
#include "level_target.h"
#include "level_island.h"
#include "active_object.h"
#include "play_event.h"
#include "starfield.h"
#include "sound_data.h"

struct level_state
{
public:

  level_state(const game_level_data& levelData, int64_t counterFrequency, const screen_render_data& renderData);

  auto Update(const level_control_state& levelControlState, int64_t counterValue) -> void;
  auto RenderTo(ID2D1RenderTarget* renderTarget, const screen_render_data& renderData) -> void;
  auto PlaySoundEffects(const global_sound_buffer_selector& soundBuffers) -> void;

  [[nodiscard]] auto IsComplete() -> bool;
  [[nodiscard]] auto PlayerIsDead() -> bool;
  [[nodiscard]] auto CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, float renderScale = 1.0) -> D2D1::Matrix3x2F;
  [[nodiscard]] auto TimedOut() -> bool;
  [[nodiscard]] auto GetPlayTimeRemaining() -> int64_t;
  [[nodiscard]] auto GetPlayTimeRemainingInSeconds() -> float;
  [[nodiscard]] auto GetUpdateInterval() -> float;

  [[nodiscard]] auto GetMouseDiagnostics() -> std::wstring
  {
    return std::format(L"world mouse: {:.2f}, {:.2f}", mouseX, mouseY);
  }

private:
  [[nodiscard]] auto GetViewRect(ID2D1RenderTarget* renderTarget) -> D2D1_RECT_F;

  const game_level_data& levelData;
  const int64_t counterFrequency;
  int64_t levelTimeLimit;

  int64_t currentTimerCount = 0;
  int64_t previousTimerCount = 0;

  D2D1::Matrix3x2F viewTransform;
  D2D1::Matrix3x2F invertedViewTransform;
  
  float mouseX = 0;
  float mouseY = 0;

  game_rect m_viewRect;
  
  bool playerShot = false;
  bool targetShot = false;

  std::shared_ptr<player_ship::data_type> playerData;
  std::list<active_object> m_activeObjects;
  std::shared_ptr<player_control_state> m_controlState;
  screen_render_brush_selector brushes;
};

#endif
