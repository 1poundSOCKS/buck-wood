#pragma once

#include "cell_size.h"
#include "geometry/pixel_geometry_loader.h"

namespace level_data
{

  enum class cell_type { empty, boundary };
  enum class object_type { none, player, enemy_stalker, enemy_random, enemy_turret };

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
  auto ConvertRawDataToCellData(raw_data rawDataItem) -> cell_data;
  auto ConvertRawDataToObjectData(raw_data rawDataItem) -> object_data;

};

auto level_data::LoadRawData(int index, auto rawDataInserter) -> bool
{
  static std::array levelData { 
    std::string { "XXXXX    XXXXXX      XX   XXX  X   XXXX" },
    std::string { "XXXX    XX   XX            X         XX" },
    std::string { "X       XX    X                       X" },
    std::string { "   1                                   " },
    std::string { "                                      X" },
    std::string { "X                                    XX" },
    std::string { "                         3      XXXXXXX" },
    std::string { "                                   XXXX" },
    std::string { "                                      X" },
    std::string { "XX            P                       X" },
    std::string { "XXXXXXXXXX                             " },
    std::string { "XXXXXXXXXXXXX                        XX" },
    std::string { "XXXXXXX                                " },
    std::string { "XXX                                  XX" },
    std::string { "                              XXXXXXXXX" },
    std::string { "         2                         XXXX" },
    std::string { "X                                    XX" },
    std::string { "                                       " },
    std::string { "XX                                     " },
    std::string { "X                                     X" },
    std::string { "                                       " },
    std::string { "                3             2        " },
    std::string { "                                       " },
    std::string { "XX                X                    " },
    std::string { "XXX              XXX     1            X" },
    std::string { "               XXXXXXX               XX" },
    std::string { "                XXXXX                 X" },
    std::string { "X              XXXXX                  X" },
    std::string { "      XX         XXX                 XX" },
    std::string { "X    XXXX         X                  XX" },
    std::string { "XXXXXXXXXX        X                  XX" },
    std::string { "XXXXXXXXX        XX    X    XX      XXX" },
    std::string { "XXXXXXXXX    2    XX  XXXX XXXX  XXXXXX" },
    std::string { "XXXXXXXXX       XXXXXXXXXXXXXXXXXXXXXXX" },
    std::string { "XXXXXXXXX      XXXXXXXXXXXXXXXXXXXXXXXX" },
    std::string { "XXXXXXXXX         X    XXXXXXXXXXXXXXXX" },
    std::string { "XXXXXXXXX                XXXXXXXXXXXXXX" },
    std::string { "XXXXXXXXXX          3    XXXXXXXXXXXXXX" },
    std::string { "XXXXXXXXX        3      XXXXXXXXXXXXXXX" },
    std::string { "XXXXXXXXXX               XXXXXXXXXXXXXX" },
    std::string { "XXXXXXXXXXXX     X    XXXXXXXXXXXXXXXXX" }
  };

  switch( index )
  {
    case 0:
      pixel_geometry_loader::imageDataToPixelData(levelData, rawDataInserter);
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

inline auto level_data::ConvertRawDataToCellData(pixel_geometry_loader::pixel_data rawDataItem) -> cell_data
{
  switch( rawDataItem.value )
  {
    case 'X':
      return { rawDataItem.column, rawDataItem.row, cell_type::empty };
    default:
      return { rawDataItem.column, rawDataItem.row, cell_type::boundary };
  }
}

inline auto level_data::ConvertRawDataToObjectData(raw_data rawDataItem) -> object_data
{
  switch( rawDataItem.value )
  {
    case 'P':
      return { rawDataItem.column, rawDataItem.row, object_type::player };
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
