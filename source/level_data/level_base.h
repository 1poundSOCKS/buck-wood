#pragma once

#include "framework.h"

class level_base
{

public:

  level_base(std::ranges::input_range auto&& data);
  virtual ~level_base() = default;

  auto Enumerate(auto&& visitor) const -> void;

protected:

  using data_type = std::vector<std::string>;
  data_type m_data;

};

inline level_base::level_base(std::ranges::input_range auto&& data)
{
  std::ranges::copy(data, std::back_inserter(m_data));
}

auto level_base::Enumerate(auto&& visitor) const -> void
{
  static constexpr int columnOffset = -1;
  static constexpr int rowOffset = -1;

  for( auto rowIndex  = 0; rowIndex < m_data.size(); ++rowIndex )
  {
    const auto& rowData = m_data[rowIndex];

    for( auto columnIndex = 0; columnIndex < rowData.size(); ++columnIndex )
    {
      visitor(columnIndex + columnOffset, rowIndex + rowOffset, rowData[columnIndex]);
    }
  }
}
