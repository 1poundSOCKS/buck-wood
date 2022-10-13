#ifndef _game_state_
#define _game_state_

#include "game_math.h"
#include "game_level.h"
#include "control_state.h"
#include "system_timer.h"

struct game_state_timer
{
  game_state_timer(const system_timer& systemTimer);

  int64_t startTicks = 0;
  int64_t stopTicks = 0;
};

struct game_state
{
  game_state(std::unique_ptr<game_level>& firstLevel);

  enum SCREEN { title, main };
  enum PLAYER_STATE { alive, dead };

  bool running;
  float speed;
  SCREEN screen;
  PLAYER_STATE playerState;
  std::unique_ptr<player_ship> player;
  std::list<std::unique_ptr<bullet>> bullets;
  std::unique_ptr<game_level> currentLevel;
  std::unique_ptr<game_state_timer> timer;
};

std::unique_ptr<game_state> CreateInitialGameState(const system_timer& systemTimer);
void UpdateGameState(game_state& gameState, const control_state& cs, const system_timer& systemTimer);
void UpdatePlayer(game_state& gameState, const control_state& controlState, float gameUpdateInterval);
void UpdateBullets(game_state& gameState, const control_state& controlState, float gameUpdateInterval, const system_timer& systemTimer);
void ResetGameState(game_state& gameState, const system_timer& systemTimer);
bool PlayerIsOutOfBounds(const game_state& gameState);
void StopGameStateTimer(game_state_timer& gameStateTimer, const system_timer& systemTimer);
float GetGameStateTimerInSeconds(const game_state_timer& gameStateTimer, const system_timer& systemTimer);

#endif
