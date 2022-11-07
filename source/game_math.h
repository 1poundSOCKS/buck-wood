#ifndef _game_math_
#define _game_math_

#include <math.h>
#include "game_objects.h"

#define PI 3.14159265
#define DEGTORAD(D)((D * PI) / 180.0)
#define RADTODEG(R)((180.0 * R) / PI)

int CalculateAngle(float x1, float y1, float x2, float y2);
bool PointsInside(const std::list<game_point>& points, const game_shape& shape);
bool PointInside(const std::list<game_point>& points, const game_shape& shape);
bool PointInside(const game_point& point, const game_shape& shape);
bool AddLineToInterceptCount(const game_line& line, const game_point& point);
float GetYIntercept(float x, const game_line& line);
void TransformPlayerShip(const player_ship& player, game_shape& shape);
void TransformPlayerShip(const player_ship& player, std::list<game_point>& transformedPoints);
float GetDistanceBetweenPoints(float x1, float y1, float x2, float y2);

#endif
