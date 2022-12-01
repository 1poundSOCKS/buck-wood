#ifndef _level_edit_screen_
#define _level_edit_screen_

#include "framework.h"
#include "control_state.h"
#include "sound_buffers.h"
#include "game_objects.h"
#include "diagnostics.h"
#include "global_state.h"
#include "drag_drop.h"
#include "screens.h"

struct level_edit_control_state
{
  bool saveChanges = false;
  bool discardChanges = false;
  bool returnToMenu = false;
  bool cancelExit = false;
  render_target_mouse_data renderTargetMouseData;
  float ratioMouseX = 0;
  float ratioMouseY = 0;
  bool nextLevel = false, previousLevel = false;
  bool addTarget = false, addObject = false;
  drag_drop_control_state dragDropControlState;
};

struct level_edit_screen_state
{
  enum drag_drop_shape_type { type_boundary, type_object, type_player, type_target };

  level_edit_screen_state(const system_timer& timer, const global_state& globalState);

  enum view_state { view_default, view_exit };
  view_state viewState = view_default;

  bool saveChanges = false;
  bool returnToMenu = false;

  const global_state& globalState;

  D2D1::Matrix3x2F viewTransform;

  float mouseX = 0, mouseY = 0;
  float levelCenterX = 0, levelCenterY = 0;
  float levelMouseX = 0, levelMouseY = 0;
  
  game_level_data_index gameLevelDataIndex;
  std::vector<std::unique_ptr<game_level_data>>::iterator currentLevelDataIterator;

  std::string levelName;
  std::wstring levelFilename;
  int levelTimeLimit = 0;
  std::unique_ptr<drag_drop_state> dragDropState;
};

void RefreshControlState(level_edit_control_state& controlState, const control_state& baseControlState);

void UpdateScreenState(level_edit_screen_state& screenState, const level_edit_control_state& controlState, const system_timer& timer);

void RenderFrame(
  ID2D1RenderTarget* renderTarget, 
  screen_render_brush_selector renderBrushSelector, 
  screen_render_text_format_selector textFormatSelector,
  const level_edit_screen_state& state);

void PlaySoundEffects(const level_edit_screen_state& screenState);

void FormatDiagnostics(std::back_insert_iterator<diagnostics_data> diagnosticsData, const level_edit_screen_state& screenState, const level_edit_control_state& controlState);

screen_status GetScreenStatus(const level_edit_screen_state& screenState);

void UpdateGlobalState(global_state& globalState, const level_edit_screen_state& screenState);

#endif
