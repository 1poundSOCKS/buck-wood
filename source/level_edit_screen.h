#ifndef _level_edit_screen_
#define _level_edit_screen_

#include <memory>
#include <list>
#include "framework/framework.h"
#include "d2d_frame.h"
#include "control_state.h"
#include "sound_buffers.h"
#include "game_objects.h"
#include "diagnostics.h"
#include "global_state.h"

struct level_edit_screen_state
{
  level_edit_screen_state(const global_state& globalState);

  mouse_cursor mouseCursor;
  bool returnToMenu = false;
  const game_level_data_index& gameLevelDataIndex;
  game_level_data_index::const_iterator currentLevelDataIterator;
  std::unique_ptr<game_level> currentLevel;
  float levelCenterX = 0, levelCenterY = 0;
  float levelMouseX = 0, levelMouseY = 0;
  game_point* closestPoint = nullptr;
  float pointDistance;
};

using level_edit_screen_state_ptr = std::unique_ptr<level_edit_screen_state>;

D2D1::Matrix3x2F CreateViewTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const level_edit_screen_state& screenState);
void RenderFrame(const d2d_frame& frame, const level_edit_screen_state& state);
void UpdateState(level_edit_screen_state& screenState, const control_state& controlState, const system_timer& timer);
void UpdateSound(const sound_buffers& soundBuffers, const level_edit_screen_state& state);
void FormatDiagnostics(diagnostics_data& diagnosticsData, const level_edit_screen_state& screenState, const control_state& controlState, const perf_data& perfData, const system_timer& timer);

#endif
