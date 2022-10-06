#ifndef _game_state_
#define _game_state_

#include "game_objects.h"
#include "game_level.h"
#include "control_state.h"

struct game_state
{
  enum SCREEN { title, main };
  enum PLAYER_STATE { alive, dead };

  game_state(std::unique_ptr<game_level>& firstLevel);
  void Update(const control_state& cs, float seconds);
  void ResetPlayer();
  bool PlayerIsOutOfBounds() const;
  void OnPlayerShoot();

  bool running;
  SCREEN screen;
  PLAYER_STATE playerState;
  std::unique_ptr<game_object> cursor;
  std::unique_ptr<game_object> player;
  std::list<std::unique_ptr<bullet>> bullets;
  std::unique_ptr<game_level> currentLevel;
};

std::unique_ptr<game_state> CreateInitialGameState();

#endif
