#ifndef _game_math_
#define _game_math_

#define PI 3.14159265
#define DEGTORAD(D)((D * PI) / 180.0)
#define RADTODEG(R)((180.0 * R) / PI)

struct game_point
{
  game_point(float x, float y) : x(x), y(y) {}

  float x = 0, y = 0;
};

struct game_line
{
  game_line(const game_point& start, const game_point& end);
  game_line(float startX, float startY, float endX, float endY);

  game_point start, end;
};

int CalculateAngle(float x1, float y1, float x2, float y2);
bool AllPointsInside(const std::vector<game_point>::const_iterator& begin, const std::vector<game_point>::const_iterator& end, const std::vector<game_line>& area);
bool AnyPointInside(const std::vector<game_point>::const_iterator& begin, const std::vector<game_point>::const_iterator& end, const std::vector<game_line>& area);
bool PointInside(const game_point& point, const std::vector<game_line>& area);
bool AddLineToInterceptCount(const game_line& line, const game_point& point);
int GetLineInterceptCount(const game_point& point, const std::vector<game_line>& lines);
int GetLineInterceptCount(const game_point& point, std::vector<game_line>::const_iterator linesBegin, std::vector<game_line>::const_iterator linesEnd);
float GetYIntercept(float x, const game_line& line);
float GetDistanceBetweenPoints(float x1, float y1, float x2, float y2);
void TransformPoints(std::vector<game_point>::const_iterator begin, std::vector<game_point>::const_iterator end, std::back_insert_iterator<std::vector<game_point>> transformedPoints, float angle, float x, float y);

void CreateConnectedLines(auto begin, auto end, auto lines, float x=0, float y=0, bool loop=true)
{
  std::transform(std::next(begin), end, begin, lines, [x, y](const auto& point2, const auto& point1)
  {
    return game_line { point1.x + x, point1.y + y, point2.x + x, point2.y + y };
  });

  if( loop )
  {
    auto last = std::prev(end);
    lines = game_line { last->x + x, last->y + y, begin->x + x, begin->y + y };
  }
};

#endif
