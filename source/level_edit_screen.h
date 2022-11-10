#ifndef _level_edit_screen_
#define _level_edit_screen_

#include <memory>
#include <list>
#include "framework/framework.h"
#include "control_state.h"
#include "sound_buffers.h"
#include "game_objects.h"
#include "diagnostics.h"
#include "global_state.h"
#include "drag_drop.h"

struct level_edit_control_state
{
  bool saveChanges = false;
  bool discardChanges = false;

  bool returnToMenu = false;
  bool cancelExit = false;
  bool leftMouseButtonDown = false;
  bool rightMouseButtonDown = false;

  render_target_mouse_data renderTargetMouseData;
  
  float ratioMouseX = 0;
  float ratioMouseY = 0;

  bool leftMouseButtonDrag = false;
  bool rightMouseButtonDrag = false;
};

struct game_point_selection
{
  game_point_selection(std::list<game_point>& points, std::list<game_point>::iterator& point, float distance);

  std::list<game_point>& points;
  std::list<game_point>::iterator point;
  float distance;
};

struct target_selection
{
  target_selection(std::list<target_edit>& targets, std::list<target_edit>::iterator& target);

  std::list<target_edit>& targets;
  std::list<target_edit>::iterator target;
};

struct level_edit_screen_state
{
  level_edit_screen_state(const global_state& globalState);

  enum view_state { view_default, view_exit };
  view_state viewState = view_default;

  bool saveChanges = false;

  const global_state& globalState;
  const d2d_brushes& brushes;
  const dwrite_text_formats& textFormats;

  D2D1::Matrix3x2F viewTransform;

  mouse_cursor mouseCursor;
  float mouseX = 0, mouseY = 0;

  bool returnToMenu = false;
  
  const game_level_data_index& gameLevelDataIndex;
  std::vector<std::unique_ptr<game_level_data>>::const_iterator currentLevelDataIterator;
  std::unique_ptr<game_level_edit> currentLevel;
  player_ship playerShip;

  float levelCenterX = 0, levelCenterY = 0;
  float levelMouseX = 0, levelMouseY = 0;
  
  std::unique_ptr<game_point_selection> closestPoint;
  std::unique_ptr<game_point_selection> dragPoint;

  std::unique_ptr<target_selection> highlightedTarget;
  std::unique_ptr<target_selection> dragTarget;

  bool playerHighlighted = false;
  bool playerDrag = false;

  drag_drop_shape dragDropShape;
};

void RefreshControlState(level_edit_control_state& controlState, const control_state& baseControlState);
void UpdateScreenState(level_edit_screen_state& screenState, const level_edit_control_state& controlState, const system_timer& timer);
void RenderFrame(const d2d_frame& frame, const level_edit_screen_state& state);
void FormatDiagnostics(diagnostics_data& diagnosticsData, const level_edit_screen_state& screenState, const level_edit_control_state& controlState);
void UpdateGlobalState(global_state& globalState, const level_edit_screen_state& screenState);

#endif
