#ifndef _game_object_
#define _game_object_

#include "game_level_data.h"

void CreatePointsForPlayer(float x, float y, float angle, std::back_insert_iterator<std::vector<game_point>> inserter);
void CreatePointsForPlayerThruster(float x, float y, float angle, std::back_insert_iterator<std::vector<game_point>> transformedPoints);
void CreatePointsForTarget(float x, float y, float size, std::back_insert_iterator<std::vector<game_point>> inserter);
void TransformPoints(std::vector<game_point>::iterator begin, std::vector<game_point>::iterator end, std::back_insert_iterator<std::vector<game_point>> transformedPoints, float angle, float x, float y);

#endif
