#include "level_editor.h"

level_editor_state::level_editor_state(const game_level_ptr& level) : level(level)
{
}

game_events_ptr UpdateLevelEditorState(level_editor_state& levelEditorState, const control_state& controlState)
{
  static const float scrollSpeed = 10.0f;

  if( controlState.mouseX < 0.1f ) levelEditorState.viewX += scrollSpeed;
  else if( controlState.mouseX > 0.9f ) levelEditorState.viewX -= scrollSpeed;

  if( controlState.mouseY < 0.1f ) levelEditorState.viewY += scrollSpeed;
  else if( controlState.mouseY > 0.9f ) levelEditorState.viewY -= scrollSpeed;

  return std::make_shared<game_events>();
}
