#pragma once

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

  auto LoadRawData(int index, auto rawDatainserter) -> bool;
  auto LoadCellData(int index, auto cellDatainserter) -> bool;
  auto LoadObjectData(int index, auto objectDatainserter) -> bool;
  auto ConvertRawDataToCellData(raw_data rawDataItem) -> cell_data;
  auto ConvertRawDataToObjectData(raw_data rawDataItem) -> object_data;

};

auto level_data::LoadRawData(int index, auto rawDataInserter) -> bool
{
  static std::array levelData { 
    std::string { "XX       XXXXXX      XX             XXX" },
    std::string { "X       XX   XX                       X" },
    std::string { "X  1   XXX    X                        " },
    std::string { "      XXXX                             " },
    std::string { "                                      X" },
    std::string { "                                     XX" },
    std::string { "                         3      XXXXXXX" },
    std::string { "                                   XXXX" },
    std::string { "                                      X" },
    std::string { "XX            P                       X" },
    std::string { "XXXXXXXXXX                             " },
    std::string { "XXXXXXXXXXXXX                          " },
    std::string { "XXXXXXX                                " },
    std::string { "XXX                                  XX" },
    std::string { "                              XXXXXXXXX" },
    std::string { "         2                         XXXX" },
    std::string { "                                     XX" },
    std::string { "                                       " },
    std::string { "                                       " },
    std::string { "                                       " },
    std::string { "                                       " },
    std::string { "                3             2        " },
    std::string { "        X                              " },
    std::string { "XX      X                              " },
    std::string { "XXX     X                1            X" },
    std::string { "       XX      XXXX                  XX" },
    std::string { "       XX       XXX                  XX" },
    std::string { "X      XX       XXX                  XX" },
    std::string { "       XXX       XX                  XX" },
    std::string { "X      XX         X                  XX" },
    std::string { "       XXX        X                  XX" },
    std::string { "       XX        XX    X    XX       XX" },
    std::string { "       XX    2    X    XXX  X        XX" },
    std::string { "X     XXX        XXXXXXXXXXXXXXXXXXXXXX" },
    std::string { "XXXX XXXX      XXXXXXXXXXXXXXXXXXXXXXXX" },
    std::string { "XXXXXXXXX                XXXXXXXXXXXXXX" },
    std::string { "XXXXXXXXX           3    XXXXXXXXXXXXXX" },
    std::string { "XXXXXXXXX                XXXXXXXXXXXXXX" },
    std::string { "XXXXXXXXX        3       XXXXXXXXXXXXXX" },
    std::string { "XXXXXXXXX                XXXXXXXXXXXXXX" },
    std::string { "XXXXXXXXX                XXXXXXXXXXXXXX" }
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
