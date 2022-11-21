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
float GetYIntercept(float x, const game_line& line);
float GetDistanceBetweenPoints(float x1, float y1, float x2, float y2);
void TransformPoints(std::vector<game_point>::const_iterator begin, std::vector<game_point>::const_iterator end, std::back_insert_iterator<std::vector<game_point>> transformedPoints, float angle, float x, float y);

template <typename T>
void CreateConnectedLines(typename std::vector<T>::const_iterator begin, 
                          typename std::vector<T>::const_iterator end, 
                          std::back_insert_iterator<std::vector<game_line>> insertIterator,
                          float x=0, float y=0)
{
  std::transform(std::next(begin), end, begin, insertIterator, [x, y](const auto& point2, const auto& point1)
  {
    game_point start(point1.x + x, point1.y + y);
    game_point end(point2.x + x, point2.y + y);
    return game_line(start, end);
  });

  typename std::vector<T>::const_iterator last = std::prev(end);

  game_point startPoint(last->x + x, last->y + y);
  game_point endPoint(begin->x + x, begin->y + y);
  insertIterator = game_line(startPoint, endPoint);
};

#endif
