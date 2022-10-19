#ifndef _game_state_
#define _game_state_

#include "game_math.h"
#include "game_level.h"
#include "control_state.h"
#include "system_timer.h"
#include "game_events.h"
#include "play_state.h"

struct game_state
{
  game_state();

  enum SCREEN { title, main };
  enum PLAYER_STATE { player_alive, player_dead };
  enum LEVEL_STATE { level_incomplete, level_complete };

  bool starting = true;
  bool running;
  float speed;
  SCREEN screen;
  std::unique_ptr<system_timer> timer;
  play_state_ptr playState;
};

std::unique_ptr<game_state> CreateInitialGameState();
game_events_ptr UpdateGameState(game_state& gameState, const control_state& cs);

#endif
