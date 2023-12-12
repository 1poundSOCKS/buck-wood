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
  game_velocity(float x, float y);
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

inline game_velocity::game_velocity(float x, float y) : m_x { x }, m_y { y }
{
}

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
