#ifndef _game_state_
#define _game_state_

#include "game_objects.h"
#include "game_level.h"
#include "control_state.h"

struct game_state
{
  enum SCREEN { title, main };

  game_state();
  void Update(const control_state& cs, float seconds);
  bool PlayerIsOutOfBounds() const;
  void OnPlayerShoot();

  bool running;
  SCREEN screen;
  game_object cursor;
  game_object player;
  std::list<std::unique_ptr<bullet>> bullets;
  std::unique_ptr<game_level> currentLevel;
};

#endif
