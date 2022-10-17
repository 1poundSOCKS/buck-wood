#ifndef _game_state_
#define _game_state_

#include "game_math.h"
#include "game_level.h"
#include "control_state.h"
#include "system_timer.h"
#include "game_events.h"

struct game_state
{
  game_state(std::unique_ptr<game_level>& firstLevel);

  enum SCREEN { title, main };
  enum PLAYER_STATE { player_alive, player_dead };
  enum LEVEL_STATE { level_incomplete, level_complete };

  game_events_ptr events;
  bool starting = true;
  bool running;
  float speed;
  SCREEN screen;
  PLAYER_STATE playerState;
  std::unique_ptr<player_ship> player;
  std::list<std::unique_ptr<bullet>> bullets;
  std::unique_ptr<game_level> currentLevel;
  LEVEL_STATE levelState = level_incomplete;
  std::unique_ptr<system_timer> timer;
  int64_t levelTimerStart = 0;
  int64_t levelTimerStop = 0;
};

std::unique_ptr<game_state> CreateInitialGameState();
void UpdateGameState(game_state& gameState, const control_state& cs);
void UpdatePlayer(game_state& gameState, const control_state& controlState, float gameUpdateInterval);
void UpdateBullets(game_state& gameState, const control_state& controlState, float gameUpdateInterval);
void ResetGameState(game_state& gameState);
bool PlayerIsOutOfBounds(const game_state& gameState);

#endif
