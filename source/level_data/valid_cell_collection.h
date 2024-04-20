#pragma once

class valid_cell
{

public:

  // using value_type = std::tuple<bool, int, int, POINT_2F, winrt::com_ptr<ID2D1TransformedGeometry>>;

  // valid_cell(auto...args) : m_value { std::forward<decltype(args)>(args)... }
  valid_cell(bool valid, int x, int y, POINT_2F position, winrt::com_ptr<ID2D1TransformedGeometry> geometry) :
    m_valid { valid }, m_x { x }, m_y { y }, m_position { position }, m_geometry { geometry }
  {
  }

  // [[nodiscard]] auto Get() const noexcept -> const value_type&
  // {
  //   return m_value;
  // }
  [[nodiscard]] auto Valid() const noexcept -> bool
  {
    return m_valid;
  }

  [[nodiscard]] auto X() const noexcept -> int
  {
    return m_x;
  }

  [[nodiscard]] auto Y() const noexcept -> int
  {
    return m_y;
  }

  [[nodiscard]] auto Position() const noexcept -> POINT_2F
  {
    return m_position;
  }

  [[nodiscard]] auto Geometry() const noexcept -> winrt::com_ptr<ID2D1TransformedGeometry>
  {
    return m_geometry;
  }

private:

  // value_type m_value;
  bool m_valid;
  int m_x;
  int m_y;
  POINT_2F m_position;
  winrt::com_ptr<ID2D1TransformedGeometry> m_geometry;

};

class valid_cell_collection
{

public:

  using value_type = std::vector<valid_cell>;
  using back_inserter_type = std::back_insert_iterator<value_type>;

  [[nodiscard]] auto Get() const noexcept -> const value_type&
  {
    return m_value;
  }

  [[nodiscard]] auto BackInserter() noexcept -> back_inserter_type
  {
    return std::back_inserter(m_value);
  }

  auto EmplaceBack(auto...args) noexcept -> void
  {
    m_value.emplace_back(std::forward<decltype(args)>(args)...);
  }

private:

  value_type m_value;

};
