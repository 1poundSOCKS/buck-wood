#include "game_objects.h"

struct game_state
{
  game_state(int frameWidth, int frameHeight);
  void Update(double seconds);

  bool started;
  game_object cursor;
  game_object player;
  std::list<std::unique_ptr<bullet>> bullets;
};
