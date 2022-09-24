#include "game_state.h"

game_state::game_state(int frameWidth, int frameHeight) 
: xPos(frameWidth / 2), yPos(frameHeight / 2), xVelocity(0), yVelocity(0), shipAngle(0)
{
}
