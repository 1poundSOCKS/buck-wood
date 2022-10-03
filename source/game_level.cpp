#include "game_level.h"

game_level::game_level(float width, float height, std::list<float> boundary)
 : width(width), height(height), boundary(boundary), playerStartPosX(width / 2), playerStartPosY(height / 2)
{
}
