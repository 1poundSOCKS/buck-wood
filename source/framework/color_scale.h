#pragma once

class color_scale;

class const_color_scale_iterator
{
public:

  friend class color_scale;

  enum class type { none, begin, end };

  using difference_type = std::ptrdiff_t;
  using value_type = D2D_COLOR_F;

  const_color_scale_iterator() = default;

  auto operator++() -> const_color_scale_iterator&;
  auto operator++(int) -> const_color_scale_iterator;
  auto operator*() const -> const D2D_COLOR_F&;
  auto operator==(const const_color_scale_iterator& i) const -> bool;

private:

  const_color_scale_iterator(const color_scale* colorScale, type t);
  auto UpdateCurrentColor() -> void;

  const color_scale* m_colorScale { nullptr };
  type m_type { type::none };
  int m_currentStep { 0 };
  D2D_COLOR_F m_currentValue { 1, 1, 1, 1 };

};

class color_scale
{

public:

  friend class const_color_scale_iterator;

  color_scale() = default;
  color_scale(const D2D_COLOR_F& start, const D2D_COLOR_F& end, size_t steps);

  [[nodiscard]] auto size() const -> size_t;

  [[nodiscard]] auto begin() const -> const_color_scale_iterator;
  [[nodiscard]] auto end() const -> const_color_scale_iterator;

private:

  D2D_COLOR_F m_start { 1, 1, 1, 1 };
  D2D_COLOR_F m_end { 0, 0, 0, 1 };
  size_t m_steps { 1 };

};
