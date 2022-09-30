#include "game_state.h"
#include <list>

game_state::game_state(int frameWidth, int frameHeight) 
: started(false)
{
  player.xPos = frameWidth / 2;
  player.yPos = frameHeight / 2;
}

bool BulletHasExpired(const std::unique_ptr<bullet>& bullet)
{
  return bullet->lifespanSeconds <= 0;
}

void game_state::Update(double seconds)
{
  player.Update(seconds);
  for(const std::unique_ptr<bullet>& bullet : bullets)
  {
    bullet->Update(seconds);
  }
  
  bullets.remove_if(BulletHasExpired);
}
