#include "game_objects.h"

struct game_state
{
  enum SCREEN { title, main };

  game_state(int frameWidth, int frameHeight);
  void Update(double seconds);

  bool running;
  SCREEN screen;
  game_object cursor;
  game_object player;
  std::list<std::unique_ptr<bullet>> bullets;
};
