#include "game_state.h"

D2D1_RECT_F game_object::GetRectangleForRender() const
{
  return D2D1::RectF(xPos + rectangle.left, yPos + rectangle.top, xPos + rectangle.right, yPos + rectangle.bottom);
}

void game_object::UpdatePosition()
{
  xPos += xVelocity;
  yPos += yVelocity;
}

game_state::game_state(int frameWidth, int frameHeight) 
: started(false)
{
  player.xPos = frameWidth / 2;
  player.yPos = frameHeight / 2;
}
