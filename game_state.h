#include "d2d_object.h"

struct game_state
{
  game_state(int frameWidth, int frameHeight);
  void UpdatePositions();

  bool started;
  d2d_object player;
  std::list<std::unique_ptr<d2d_object>> bullets;
  double targetPosX, targetPosY;
};
