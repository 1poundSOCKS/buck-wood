#ifndef _game_math_
#define _game_math_

// #include <math.h>
#include "game_objects.h"

#define PI 3.14159265
#define DEGTORAD(D)((D * PI) / 180.0)
#define RADTODEG(R)((180.0 * R) / PI)

int CalculateAngle(float x1, float y1, float x2, float y2);
bool PointsInside(const std::vector<game_point>::const_iterator& begin, const std::vector<game_point>::const_iterator& end, const std::vector<game_line>& area);
bool PointInside(const std::vector<game_point>::const_iterator& begin, const std::vector<game_point>::const_iterator& end, const std::vector<game_line>& area);
bool PointInside(const game_point& point, const std::vector<game_line>& area);
bool AddLineToInterceptCount(const game_line& line, const game_point& point);
float GetYIntercept(float x, const game_line& line);
float GetDistanceBetweenPoints(float x1, float y1, float x2, float y2);
void TransformPoints(std::vector<game_point>::iterator begin, std::vector<game_point>::iterator end, std::back_insert_iterator<std::vector<game_point>> transformedPoints, float angle, float x, float y);

#endif
