#ifndef _level_edit_screen_state_
#define _level_edit_screen_state_

#include "game_level_data.h"
#include "input_state.h"
#include "drag_drop.h"

struct level_edit_screen_state
{
  enum drag_drop_shape_type { type_boundary, type_object, type_player, type_target };

  level_edit_screen_state(const game_level_data_index& gameLevelDataIndex);

  enum view_state { view_default, view_exit };
  view_state viewState = view_default;

  bool saveChanges = false;
  bool returnToMenu = false;

  render_target_mouse_data renderTargetMouseData;

  D2D1::Matrix3x2F viewTransform;

  float levelCenterX = 0, levelCenterY = 0;
  float levelMouseX = 0, levelMouseY = 0;
  
  game_level_data_index gameLevelDataIndex;
  game_level_data_index::iterator currentLevelDataIterator;

  std::string levelName;
  std::wstring levelFilename;
  int levelTimeLimit = 0;
  std::unique_ptr<drag_drop_state> dragDropState;
};

#endif
