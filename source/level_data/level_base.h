#pragma once

#include "framework.h"
#include "level_types.h"

class level_base
{

public:

  level_base(std::ranges::input_range auto&& data);
  virtual ~level_base() = default;

  auto EnumerateCells(auto&& visitor) -> void;
  auto EnumerateItems(auto&& visitor) -> void;

private:

  static [[nodiscard]] auto CellType(char cellData) -> level_cell_type;
  static [[nodiscard]] auto ItemType(char cellData) -> level_item_type;

private:

  using data_type = std::vector<std::string>;
  data_type m_data;

};

inline level_base::level_base(std::ranges::input_range auto&& data)
{
  std::ranges::copy(data, std::back_inserter(m_data));
}

auto level_base::EnumerateCells(auto&& visitor) -> void
{
  for( auto rowIndex  = 0; rowIndex < m_data.size(); ++rowIndex )
  {
    const auto& rowData = m_data[rowIndex];

    for( auto columnIndex = 0; columnIndex < rowData.size(); ++columnIndex )
    {
      visitor(columnIndex, rowIndex, CellType(rowData[columnIndex]));
    }
  }
}

auto level_base::EnumerateItems(auto&& visitor) -> void
{
  for( auto rowIndex  = 0; rowIndex < m_data.size(); ++rowIndex )
  {
    const auto& rowData = m_data[rowIndex];

    for( auto columnIndex = 0; columnIndex < rowData.size(); ++columnIndex )
    {
      visitor(columnIndex, rowIndex, ItemType(rowData[columnIndex]));
    }
  }
}

inline [[nodiscard]] auto level_base::CellType(char cellData) -> level_cell_type
{
  switch( cellData )
  {
    case '0':
      return level_cell_type::none;
    case 'E':
      return level_cell_type::exit;
    default:
      return level_cell_type::floor;
  }
}

inline [[nodiscard]] auto level_base::ItemType(char cellData) -> level_item_type
{
  switch( cellData )
  {
    case 'P':
      return level_item_type::portal;
    case '1':
      return level_item_type::enemy_type_one;
    case '2':
      return level_item_type::enemy_type_two;
    case '3':
      return level_item_type::enemy_type_three;
    default:
      return level_item_type::none;
  }
}
