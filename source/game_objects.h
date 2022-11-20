#ifndef _game_object_
#define _game_object_

#include "game_level_data.h"

void CreatePointsForPlayer(std::back_insert_iterator<std::vector<game_point>> inserter);
void CreatePointsForPlayerThruster(float x, float y, float angle, std::back_insert_iterator<std::vector<game_point>> transformedPoints);
void CreatePointsForTarget(float x, float y, float size, std::back_insert_iterator<std::vector<game_point>> inserter);

#endif
