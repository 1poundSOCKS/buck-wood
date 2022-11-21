#ifndef _game_object_
#define _game_object_

#include "game_level_data.h"

static const float defaultTargetSize = 40;

void CreatePointsForPlayer(std::back_insert_iterator<std::vector<game_point>> inserter);
void CreatePointsForPlayerThruster(std::back_insert_iterator<std::vector<game_point>> transformedPoints);
void CreatePointsForTarget(float size, std::back_insert_iterator<std::vector<game_point>> inserter);

#endif
