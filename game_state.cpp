#include "game_state.h"
#include <list>

game_state::game_state(int frameWidth, int frameHeight) 
: started(false)
{
  player.xPos = frameWidth / 2;
  player.yPos = frameHeight / 2;
  // bullet = std::make_unique<d2d_object>();
}

bool BulletHasLeftScreen(const std::unique_ptr<d2d_object>& bullet) {
  return bullet->yPos < 0 || bullet->yPos > 1000 || bullet->xPos < 0 || bullet->xPos > 1000
  ? true : false;
}

void game_state::UpdatePositions()
{
  player.UpdatePosition();
  for(const std::unique_ptr<d2d_object>& bullet : bullets)
  {
    bullet->UpdatePosition();
  }
  
  // bullets.remove_if(BulletHasLeftScreen);

  // bullet->UpdatePosition();
}
