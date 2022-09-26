#include "game_state.h"
#include <list>

game_state::game_state(int frameWidth, int frameHeight) 
: started(false), targetPosX(0), targetPosY(0)
{
  player.xPos = frameWidth / 2;
  player.yPos = frameHeight / 2;
}

bool BulletHasLeftScreen(const std::unique_ptr<d2d_object>& bullet) { return bullet->yPos < 0 ? true : false; }

void game_state::UpdatePositions()
{
  player.UpdatePosition();
  for(const std::unique_ptr<d2d_object>& bullet : bullets)
  {
    bullet->UpdatePosition();
  }
  
  bullets.remove_if(BulletHasLeftScreen);
}
