#ifndef _game_state_
#define _game_state_

#include "game_math.h"
#include "game_level.h"
#include "control_state.h"
#include "system_timer.h"
#include "game_events.h"
#include "play_state.h"
#include "level_editor.h"

struct game_state
{
  game_state();

  enum SCREEN { screen_title, screen_play, screen_level_editor };
  enum PLAYER_STATE { player_alive, player_dead };
  enum LEVEL_STATE { level_incomplete, level_complete };

  bool starting = true;
  bool running = true;
  SCREEN screen = screen_title;
  play_state_ptr playState;
  level_editor_state_ptr levelEditorState;
  game_level_ptr gameLevel;
};

std::unique_ptr<game_state> CreateInitialGameState();
game_events_ptr UpdateGameState(game_state& gameState, const control_state& cs, const system_timer& timer);

#endif
