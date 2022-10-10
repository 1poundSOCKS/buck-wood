#ifndef _game_state_
#define _game_state_

#include "game_objects.h"
#include "game_level.h"
#include "control_state.h"

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
};

std::unique_ptr<game_state> CreateInitialGameState();
void UpdateGameState(game_state& gameState, const control_state& cs, float seconds);
void UpdatePlayer(game_state& gameState, const control_state& controlState, float seconds);
void UpdateBullets(game_state& gameState, const control_state& controlState, float seconds);
void ResetGameState(game_state& gameState);
bool PlayerIsOutOfBounds(const game_state& gameState);

#endif
