#include "game_state.h"

game_state::game_state(int frameWidth, int frameHeight) 
: started(false)
{
  player.xPos = frameWidth / 2;
  player.yPos = frameHeight / 2;
}

void game_state::UpdatePositions()
{
  player.UpdatePosition();
  for(const std::unique_ptr<d2d_object>& bullet : bullets)
  {
    bullet->UpdatePosition();
  }
}
