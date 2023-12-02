#pragma once

class setting_slider
{

public:

  setting_slider() = default;
  setting_slider(int min, int max, int value);
  setting_slider(int min, int max, int value, const D2D1_RECT_F& rect);
  virtual ~setting_slider() = default;

  [[nodiscard]] virtual auto Min() const -> int;
  [[nodiscard]] virtual auto Max() const -> int;
  [[nodiscard]] virtual auto Value() const -> int;

  virtual auto Increment() -> int;
  virtual auto Decrement() -> int;

  [[nodiscard]] auto HoverState() const -> bool;
  auto SetHoverState(bool value) -> void;

  virtual auto Select() -> void;
  virtual auto Unselect() -> void;

  [[nodiscard]] auto Rect() const -> const D2D1_RECT_F&;

  auto Resize(const D2D1_RECT_F& rect) -> void;

  [[nodiscard]] auto Name() const -> const std::wstring&;

private:

  int m_min { 0 };
  int m_max { 10 };
  int m_value { 10 };
  D2D1_RECT_F m_rect { 0, 0, 0, 0 };
  bool m_hover { false };

protected:

  std::wstring m_name;

};
