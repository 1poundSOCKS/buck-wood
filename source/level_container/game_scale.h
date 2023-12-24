#pragma once

class game_scale
{
public:

  game_scale(float width, float height) : m_width { width }, m_height { height }
  {
  }

  [[nodiscard]] auto width() const -> float
  {
    return m_width;
  }

  [[nodiscard]] auto height() const -> float
  {
    return m_height;
  }

private:

  float m_width { 1.0 };
  float m_height { 1.0 };

};
