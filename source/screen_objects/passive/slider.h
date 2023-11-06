#pragma once

class slider
{

public:

  slider() = default;
  slider(const D2D1_RECT_F& rect);

  [[nodiscard]] auto Rect() const -> const D2D1_RECT_F&;

  auto Resize(const D2D1_RECT_F& rect) -> void;

private:

  D2D1_RECT_F m_rect { 0, 0, 0, 0 };

};

inline slider::slider(const D2D1_RECT_F& rect) : m_rect { rect }
{
}

[[nodiscard]] inline auto slider::Rect() const -> const D2D1_RECT_F&
{
  return m_rect;
}

inline auto slider::Resize(const D2D1_RECT_F& rect) -> void
{
  m_rect = rect;
}
