#pragma once

class valid_cell
{

public:

  using value_type = std::tuple<bool, int, int, POINT_2F, winrt::com_ptr<ID2D1TransformedGeometry>>;

  valid_cell(auto...args) : m_value { std::forward<decltype(args)>(args)... }
  {
  }

  [[nodiscard]] auto Get() const noexcept -> const value_type&
  {
    return m_value;
  }

private:

  value_type m_value;

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
