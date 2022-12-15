#ifndef _collisions_
#define _collisions_

#include "geometry.h"

int CalculateAngle(float x1, float y1, float x2, float y2);
bool AllPointsInside(const std::vector<game_point>::const_iterator& begin, const std::vector<game_point>::const_iterator& end, const std::vector<game_line>& area);
bool AnyPointInside(const std::vector<game_point>::const_iterator& begin, const std::vector<game_point>::const_iterator& end, const std::vector<game_line>& area);
bool PointInside(const game_point& point, const std::vector<game_line>& area);
bool AddLineToInterceptCount(const game_line& line, const game_point& point);
int GetLineInterceptCount(const game_point& point, const std::vector<game_line>& lines);
int GetLineInterceptCount(const game_point& point, std::vector<game_line>::const_iterator linesBegin, std::vector<game_line>::const_iterator linesEnd);
float GetYIntercept(float x, const game_line& line);

[[nodiscard]] bool GetPointInsideShapeFlag(float x, float y, auto linesBegin, auto linesEnd)
{
  return ( GetLineInterceptCount({x, y}, linesBegin, linesEnd) % 2 == 1 );
}

#endif
