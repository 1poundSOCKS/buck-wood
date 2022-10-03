#include "game_state.h"
#include <list>

game_state::game_state()
: running(true), screen(title)
{
  cursor.size = 5.0;
}

bool BulletHasExpired(const std::unique_ptr<bullet>& bullet)
{
  return bullet->lifespanSeconds <= 0;
}

void game_state::Update(float seconds)
{
  player.Update(seconds);
  for(const std::unique_ptr<bullet>& bullet : bullets)
  {
    bullet->Update(seconds);
  }
  
  bullets.remove_if(BulletHasExpired);
}
