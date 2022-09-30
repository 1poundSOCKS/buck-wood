#include "d2d_object.h"

struct game_state
{
  game_state(int frameWidth, int frameHeight);
  void Update(double seconds);

  bool started;
  d2d_object cursor;
  d2d_object player;
  std::list<std::unique_ptr<bullet>> bullets;
};
