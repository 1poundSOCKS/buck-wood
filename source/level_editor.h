#ifndef _level_editor_
#define _level_editor_

#include "d2d_frame.h"
#include "game_level.h"
#include "control_state.h"
#include "game_events.h"

struct level_editor_state
{
  level_editor_state(const game_level_ptr& level);

  const game_level_ptr level;
  float viewX = 0, viewY = 0;
};

using level_editor_state_ptr = std::unique_ptr<level_editor_state>;

game_events_ptr UpdateLevelEditorState(level_editor_state& levelEditorState, const control_state& controlState);

#endif
