#pragma once

#define PI 3.14159265f
#define DEGTORAD(D)((D * PI) / 180.0f)
#define RADTODEG(R)((180.0f * R) / PI)

[[nodiscard]] inline auto CalculateVectorX(float value, float angle) -> float
{
  return value * sin(DEGTORAD(angle));
}

[[nodiscard]] inline auto CalculateVectorY(float value, float angle) -> float
{
  return value * cos(DEGTORAD(angle));
}

inline auto CalculateAngle(float x1, float y1, float x2, float y2) -> float
{
  float radians = atan2(y2 - y1, x2 - x1);
  float degrees = RADTODEG(radians);
  return degrees + 90.0f;
}

inline float GetDistanceBetweenPoints(float x1, float y1, float x2, float y2)
{
  float cx = x2 - x1;
  float cy = y2 - y1;
  return sqrt( cx * cx + cy * cy );
}

struct game_point
{
  float x = 0;
  float y = 0;

  [[nodiscard]] auto DistanceTo(game_point p) const -> float;
  [[nodiscard]] auto AngleTo(game_point p) const -> float;
};

[[nodiscard]] inline auto game_point::DistanceTo(game_point p) const -> float
{
  return GetDistanceBetweenPoints(x, y, p.x, p.y);
}

[[nodiscard]] inline auto game_point::AngleTo(game_point p) const -> float
{
  return CalculateAngle(x, y, p.x, p.y);
}

struct game_line
{
  game_point start;
  game_point end;
};

struct game_rect
{
  game_rect();
  game_rect(float left, float top, float right, float bottom);
  game_rect(game_point topLeft, game_point bottomRight);
  game_rect(D2D1_RECT_F rect);

  [[nodiscard]] auto Width() const -> float;
  [[nodiscard]] auto Height() const -> float;
  [[nodiscard]] auto CentrePoint() const -> game_point;

  auto Resize(float widthRatio, float heightRation) -> void;

  game_point topLeft = { 0, 0 };
  game_point bottomRight = { 0, 0 };
};

class game_angle
{
public:

  game_angle() = default;

  game_angle(float value) : m_value { value }
  {
  }

  operator float() const
  {
    return m_value;
  }

  auto operator +=(float value) -> float
  {
    m_value += value;
    
    while( m_value < 0 )
    {
      m_value += 360;
    }
    
    while( m_value >= 360 )
    {
      m_value -= 360;
    }

    return m_value;
  }

private:

  float m_value { 0 };

};

struct game_velocity
{
  game_velocity() = default;
  // game_velocity(float x, float y);
  game_velocity(const game_angle& direction, float speed);

  auto operator+=(const game_velocity& increase) -> game_velocity&;
  [[nodiscard]] auto operator+(const game_velocity& increase) const -> game_velocity;
  auto Update(float xMultiplier, float yMultipler) -> void;

  [[nodiscard]] auto x() const -> float;
  [[nodiscard]] auto y() const -> float;
  [[nodiscard]] auto Speed() const -> float;

  auto Accelerate(float cx, float cy) -> void;

private:

  float m_x { 0 };
  float m_y { 0 };
};

// inline game_velocity::game_velocity(float xVal, float yVal) : x { xVal }, y { yVal }
// {
// }

inline game_velocity::game_velocity(const game_angle& direction, float speed)
{
  m_x = speed * sin(DEGTORAD(direction));
  m_y = -speed * cos(DEGTORAD(direction));
}

inline auto game_velocity::operator+=(const game_velocity& increase) -> game_velocity&
{
  m_x += increase.m_x;
  m_y += increase.m_y;
  return *this;
}

inline [[nodiscard]] auto game_velocity::operator+(const game_velocity& value) const -> game_velocity
{
  return { m_x + value.m_x, m_y + value.m_y };
}

inline auto game_velocity::Update(float xMultiplier, float yMultiplier) -> void
{
  m_x *= xMultiplier;
  m_y *= yMultiplier;
}

inline [[nodiscard]] auto game_velocity::x() const -> float
{
  return m_x;
}

inline [[nodiscard]] auto game_velocity::y() const -> float
{
  return m_y;
}

inline [[nodiscard]] auto game_velocity::Speed() const -> float
{
  return sqrt(m_x * m_x + m_y * m_y);
}

inline auto game_velocity::Accelerate(float cx, float cy) -> void
{
  m_x += cx;
  m_y += cy;
}

void CreateConnectedLines(auto begin, auto end, auto lines, bool loop=true)
{
  if( std::distance(begin, end) > 0 )
  {
    std::transform(std::next(begin), end, begin, lines, [](const auto& point2, const auto& point1)
    {
      return game_line { point1.x, point1.y, point2.x, point2.y };
    });

    if( loop )
    {
      auto last = std::prev(end);
      lines = game_line { last->x, last->y, begin->x, begin->y };
    }
  }
};

void CreateConnectedLines(game_rect rect, auto lines)
{
  auto points = std::array {
    rect.topLeft,
    game_point { rect.bottomRight.x, rect.topLeft.y },
    rect.bottomRight,
    game_point { rect.topLeft.x, rect.bottomRight.y }
  };

  CreateConnectedLines(points.cbegin(), points.cend(), lines);
};

struct game_closed_object
{
  game_closed_object()
  {
  }

  template <typename point_iterator_type>
  game_closed_object(point_iterator_type begin, point_iterator_type end)
  {
    Load(begin, end);
  }

  template <typename point_iterator_type>
  auto Load(point_iterator_type begin, point_iterator_type end) -> void
  {
    Clear();
    std::copy(begin, end, std::back_inserter(points));
    CreateConnectedLines(points.cbegin(), points.cend(), std::back_inserter(lines));
  }

  auto Clear() -> void
  {
    points.clear();
    lines.clear();
  }

  auto Reserve(size_t size) -> void
  {
    points.reserve(size);
    lines.reserve(size);
  }

  auto GetBoundingRect() const -> game_rect;

  auto operator+=(game_point&& point) -> void
  {
    points.emplace_back(point);
  }

  auto operator+=(game_point point) -> void
  {
    points.push_back(point);
  }

  auto Finalize() -> void
  {
    CreateConnectedLines(points.cbegin(), points.cend(), std::back_inserter(lines));
  }

  std::vector<game_point> points;
  std::vector<game_line> lines;
};

struct game_open_object
{
  std::vector<game_point> points;
  std::vector<game_line> lines;
};

[[nodiscard]] auto GetBoundingRect(game_line line) -> game_rect ;
[[nodiscard]] auto GetBoundingRect(game_rect rect1, game_rect rect2) -> game_rect;
[[nodiscard]] auto DoOverlap(game_rect rect1, game_rect rect2) -> bool;
[[nodiscard]] auto GetCentrePoint(game_rect rect) -> game_point;

void TransformPoints(auto begin, auto end, auto transformedPoints, const D2D1::Matrix3x2F& transform)
{
  std::transform(begin, end, transformedPoints, [transform](const auto& point){
    auto outPoint = transform.TransformPoint({point.x, point.y});
    return decltype(transformedPoints)::container_type::value_type ( outPoint.x, outPoint.y );
  });
}

[[nodiscard]] auto LoadOpenObject(auto beginPoint, auto endPoint) -> game_open_object
{
  game_open_object openObject;
  std::copy(beginPoint, endPoint, std::back_inserter(openObject.points));
  CreateConnectedLines(openObject.points.cbegin(), openObject.points.cend(), std::back_inserter(openObject.lines), false);
  return openObject;
}
