#pragma once

#include "cell_size.h"
#include "geometry/pixel_geometry_loader.h"

namespace level_data
{

  constexpr static std::array levelData_1 { 
    std::string_view { "XXXXX    XXXXXX      XX   XXX  X   XXXX" },
    std::string_view { "XXXX    XX   XX            X         XX" },
    std::string_view { "X       XX    X                       X" },
    std::string_view { "   1                                   " },
    std::string_view { "                                      X" },
    std::string_view { "X          O                         XX" },
    std::string_view { "                         3      XXXXXXX" },
    std::string_view { "                                   XXXX" },
    std::string_view { "                                      X" },
    std::string_view { "XX            P                       X" },
    std::string_view { "XXXXXXXXXX                             " },
    std::string_view { "XXXXXXXXXXXXX                        XX" },
    std::string_view { "XXXXXXX                                " },
    std::string_view { "XXX                                  XX" },
    std::string_view { "                              XXXXXXXXX" },
    std::string_view { "         2                         XXXX" },
    std::string_view { "X                                    XX" },
    std::string_view { "                                       " },
    std::string_view { "XX                                     " },
    std::string_view { "X                                     X" },
    std::string_view { "                                       " },
    std::string_view { "                3             2        " },
    std::string_view { "                                       " },
    std::string_view { "XX                X                    " },
    std::string_view { "XXX              XXX     1            X" },
    std::string_view { "               XXXXXXX               XX" },
    std::string_view { "                XXXXX                 X" },
    std::string_view { "X              XXXXX                  X" },
    std::string_view { "      XX         XXX                 XX" },
    std::string_view { "X    XXXX         X                  XX" },
    std::string_view { "XXXXXXXXXX        X                  XX" },
    std::string_view { "XXXXXXXXX        XX    X    XX      XXX" },
    std::string_view { "XXXXXXXXX    2    XX  XXXX XXXX  XXXXXX" },
    std::string_view { "XXXXXXXXX       XXXXXXXXXXXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXX      XXXXXXXXXXXXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXX         X    XXXXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXX                XXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXX          3    XXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXX        3      XXXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXX               XXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXXXX     X    XXXXXXXXXXXXXXXXX" }
  };

  constexpr static std::array levelData_0 { 
    std::string_view { "XXXXX    XXXXXX      XX   XXX  X   XXXX" },
    std::string_view { "XXXX    XX   XX            X         XX" },
    std::string_view { "X       XX    X                       X" },
    std::string_view { "   1                             2     " },
    std::string_view { "                                      X" },
    std::string_view { "X                                    XX" },
    std::string_view { "                         3      XXXXXXX" },
    std::string_view { "                                   XXXX" },
    std::string_view { "                                      X" },
    std::string_view { "XX            P                       X" },
    std::string_view { "XXXXXXXXXX                             " },
    std::string_view { "XXXXXXXXXXXXX                O       XX" },
    std::string_view { "XXXXXXX                                " },
    std::string_view { "XXX                                  XX" }
  };

  constexpr int levelCount = 2;

  enum class cell_type { empty, boundary };
  enum class object_type { none, player, power_up, enemy_stalker, enemy_random, enemy_turret };

  using raw_data = pixel_geometry_loader::pixel_data;

  struct cell_data
  {
    int column;
    int row;
    cell_type type;
  };

  struct object_data
  {
    int column;
    int row;
    object_type type;
  };

  inline cell_size cellSize { 250, 250 };

  auto LoadRawData(int index, auto rawDatainserter) -> bool;
  auto LoadCellData(int index, auto cellDatainserter) -> bool;
  auto LoadObjectData(int index, auto objectDatainserter) -> bool;
  auto LoadBoundaryData(int index, auto pointDatainserter) -> bool;
  constexpr auto ConvertRawDataToCellData(raw_data rawDataItem) -> cell_data;
  constexpr auto ConvertRawDataToObjectData(raw_data rawDataItem) -> object_data;

};

auto level_data::LoadRawData(int index, auto rawDataInserter) -> bool
{
  switch( index )
  {
    case 0:
      pixel_geometry_loader::imageDataToPixelData(levelData_0, rawDataInserter);
      return true;
    case 1:
      pixel_geometry_loader::imageDataToPixelData(levelData_1, rawDataInserter);
      return true;
    default:
      return false;
  }
}

auto level_data::LoadCellData(int index, auto cellDataInserter) -> bool
{
  std::vector<raw_data> rawData;

  if( LoadRawData(index, std::back_inserter(rawData)) )
  {
    std::ranges::transform(rawData, cellDataInserter, [](auto&& rawDataItem) -> cell_data { return ConvertRawDataToCellData(rawDataItem); });
    return true;
  }
  else
  {
    return false;
  }
}

auto level_data::LoadObjectData(int index, auto objectDataInserter) -> bool
{
  std::vector<raw_data> rawData;

  if( LoadRawData(index, std::back_inserter(rawData)) )
  {
    std::ranges::transform(rawData, objectDataInserter, [](auto&& rawDataItem) -> object_data { return ConvertRawDataToObjectData(rawDataItem); });
    return true;
  }
  else
  {
    return false;
  }
}

auto level_data::LoadBoundaryData(int index, auto pointDatainserter) -> bool
{
  std::vector<raw_data> rawData;

  if( LoadRawData(index, std::back_inserter(rawData)) )
  {
    std::vector<POINT_2I> pointData;
    pixel_geometry_loader::pixelDataToOrderedPointData(rawData, cellSize, std::back_inserter(pointData), [](auto&& pixelData) -> bool { return pixelData.value != 'X'; });
    std::ranges::transform(pointData, pointDatainserter, [](POINT_2I pointData) -> POINT_2F { return ToFloat(pointData); });
    return true;
  }
  else
  {
    return false;
  }
}

inline constexpr auto level_data::ConvertRawDataToCellData(pixel_geometry_loader::pixel_data rawDataItem) -> cell_data
{
  switch( rawDataItem.value )
  {
    case 'X':
      return { rawDataItem.column, rawDataItem.row, cell_type::empty };
    default:
      return { rawDataItem.column, rawDataItem.row, cell_type::boundary };
  }
}

inline constexpr auto level_data::ConvertRawDataToObjectData(raw_data rawDataItem) -> object_data
{
  switch( rawDataItem.value )
  {
    case 'P':
      return { rawDataItem.column, rawDataItem.row, object_type::player };
    case 'O':
      return { rawDataItem.column, rawDataItem.row, object_type::power_up };
    case '1':
      return { rawDataItem.column, rawDataItem.row, object_type::enemy_stalker };
    case '2':
      return { rawDataItem.column, rawDataItem.row, object_type::enemy_random };
    case '3':
      return { rawDataItem.column, rawDataItem.row, object_type::enemy_turret };
    default:
      return { rawDataItem.column, rawDataItem.row, object_type::none };
  }
}
