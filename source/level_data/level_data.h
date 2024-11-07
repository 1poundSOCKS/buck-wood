#pragma once

#include "geometry/image_data.h"

namespace level_data
{

  constexpr int levelCount = 3;

  enum class cell_type { empty, boundary };
  enum class object_type { none, player, power_up, enemy_stalker, enemy_random, enemy_turret, enemy_guard };

  constexpr auto ConvertToCellData(char value) -> cell_type;
  constexpr auto ConvertToObjectData(char value) -> object_type;

  auto CopyCellData(int index, auto&& inserter) -> void;
  auto CopyObjectData(int index, auto&& inserter) -> void;

  constexpr static std::array levelData_0 { 
    std::string_view { "XXXXXXXXXXXXXXXXX   XXX  X   XXXX" },
    std::string_view { "XXXXXXXXXXXXXX      X         XXX" },
    std::string_view { "XXXXXXXXXXXXX                  XX" },
    std::string_view { "XXXXXX     X             O       " },
    std::string_view { "XXXX                            X" },
    std::string_view { "XXXXXX        X                XX" },
    std::string_view { "XXXXX      X              XXXXXXX" },
    std::string_view { "XXXXXXXX                XXXXXXXXX" },
    std::string_view { "XXXXXXXXXX          XXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXX           XXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXXXX       XXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXX           XXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXX      1   XXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXXX          XXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXX   1       XXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXX          XXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXXXX            XXXXXXXXX" },
    std::string_view { "XXXXXXXXXX                 XXXXXX" },
    std::string_view { "XXXXXXXXXXX               XXXXXXX" },
    std::string_view { "XXXXXXXXXX                XXXXXXX" },
    std::string_view { "XXXXXXXX         XX X        XXXX" },
    std::string_view { "XXXXXXXX       XXXXXXX          X" },
    std::string_view { "XXXXXX           XX            XX" },
    std::string_view { "XXXXXXX           X              " },
    std::string_view { "XXXXXXXX                 P     XX" },
    std::string_view { "XXXXXXXX                       XX" },
    std::string_view { "XXXXXXXX                       XX" },
    std::string_view { "XXXXXXXXXXXXX   X  X X    XXXXXXX" }
  };

  constexpr static std::array levelData_1 { 
    std::string_view { "XXXXX    XXXXXXXXXXXXXX   XXX  X   XXXX" },
    std::string_view { "XXXX    XX   XXXXXX        X        XXX" },
    std::string_view { "X       XX                            X" },
    std::string_view { "                              O        " },
    std::string_view { "                                      X" },
    std::string_view { "X      P                             XX" },
    std::string_view { "             XXX                XXXXXXX" },
    std::string_view { "                             XXXXXXXXXX" },
    std::string_view { "                                  XXXXX" },
    std::string_view { "XX                2                   X" },
    std::string_view { "XXXXXXXXXXX                            " },
    std::string_view { "XXXXXXXXXXXXXXXXXXXXXXX        O     XX" },
    std::string_view { "XXXXXXX  X    X  X                     " },
    std::string_view { "XXX                        XXXX    XXXX" },
    std::string_view { "                           XXXXXXXXXXXX" },
    std::string_view { "X                     X            XXXX" },
    std::string_view { "X          O                         XX" },
    std::string_view { "                                       " },
    std::string_view { "XXX                                    " },
    std::string_view { "X                    4                X" },
    std::string_view { "XX                         XXX         " },
    std::string_view { "X         O               XXXXXX       " },
    std::string_view { "                           XXXX        " },
    std::string_view { "XX                X       XXXX         " },
    std::string_view { "XXXXXXX          XXX        XX        X" },
    std::string_view { "XXXXXXXXX      XXXXXXX               XX" },
    std::string_view { "XXXXXXXXX       XXXXX                 X" },
    std::string_view { "XXXXXXX        XXXXX           O      X" },
    std::string_view { "XXXXXXX          XXX        O        XX" },
    std::string_view { "XXXXXXXXX  1      X                  XX" },
    std::string_view { "XXXXXXXXXX        X                  XX" },
    std::string_view { "XXXXXXXXX        XX    X    XX      XXX" },
    std::string_view { "XXXXXXXXX    1    XX  XXXX XXXX  XXXXXX" },
    std::string_view { "XXXXXXXXX       XXXXXXXXXXXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXX      XXXXXXXXXXXXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXX         X    XXXXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXX                XXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXX          O    XXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXX               XXXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXX               XXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXXXX     X    XXXXXXXXXXXXXXXXX" }
  };

  constexpr static std::array levelData_2 { 
    std::string_view { "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" },
    std::string_view { "XXXX    XXXXXXXXXXXXXXXXXXXXXXXXX" },
    std::string_view { "X         XXXXXXXXXXXXXXXXXXXXXXX" },
    std::string_view { "                         XXXXXXXX" },
    std::string_view { "    P                       XXXXX" },
    std::string_view { "X                O          XXXXX" },
    std::string_view { "XXXXX                       XXXXX" },
    std::string_view { "XXXXXXXXXXXXXX         X    XXXXX" },
    std::string_view { "XXXXXXXXXXXXXXXXXXXXXXXXX   XXXXX" },
    std::string_view { "XXXXXXXXXXX         XXXXX   XXXXX" },
    std::string_view { "XXXXXXXXXX           XXX    XXXXX" },
    std::string_view { "XXXXXXXXXX     3     XXXX   XXXXX" },
    std::string_view { "XXXXXXXXXXX         XXX     XXXXX" },
    std::string_view { "XXXXXXXXXXXXXX               XXXX" },
    std::string_view { "XXXXXXXXXXXXX      O          XXX" },
    std::string_view { "XXXXXXXXXXX                XXXXXX" },
    std::string_view { "XXXXXXXXXXXX    XXXXXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXX       XXXXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXXX      XXXXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXX       XXXXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXXXXX   XXXXXXXXXXXXXXXXX" },
    std::string_view { "XXXXXXXXXX           XXXXXXXXXXXX" },
    std::string_view { "XXXXXX                         XX" },
    std::string_view { "XXXXXXX    O            O     XXX" },
    std::string_view { "XXXXXXXX         XXX           XX" },
    std::string_view { "XXXXXXXXXXXXX   XXXXXX    XXXXXXX" }
  };

};

constexpr auto level_data::ConvertToCellData(char value) -> cell_type
{
  switch( value )
  {
    case 'X':
      return cell_type::boundary;
    default:
      return cell_type::empty;
  }
}

constexpr auto level_data::ConvertToObjectData(char value) -> object_type
{
  switch( value )
  {
    case 'P':
      return object_type::player;
    case 'O':
      return object_type::power_up;
    case '1':
      return object_type::enemy_stalker;
    case '2':
      return object_type::enemy_random;
    case '3':
      return object_type::enemy_turret;
    case '4':
      return object_type::enemy_guard;
    default:
      return object_type::none;
  }
}

auto level_data::CopyCellData(int index, auto &&inserter) -> void
{
  switch( index )
  {
    case 0:
      std::ranges::transform(image_data { levelData_0 }, inserter, [](auto&& dataItem) -> std::tuple<int, int, cell_type>
      {
        auto&& [column, row, value] = dataItem;
        return { column, row, ConvertToCellData(value) };
      });
      break;
    case 1:
      std::ranges::transform(image_data { levelData_1 }, inserter, [](auto&& dataItem) -> std::tuple<int, int, cell_type>
      {
        auto&& [column, row, value] = dataItem;
        return { column, row, ConvertToCellData(value) };
      });
      break;
    case 2:
      std::ranges::transform(image_data { levelData_2 }, inserter, [](auto&& dataItem) -> std::tuple<int, int, cell_type>
      {
        auto&& [column, row, value] = dataItem;
        return { column, row, ConvertToCellData(value) };
      });
      break;
  }
}

auto level_data::CopyObjectData(int index, auto &&inserter) -> void
{
  switch( index )
  {
    case 0:
      std::ranges::transform(image_data { levelData_0 }, inserter, [](auto&& dataItem) -> std::tuple<int, int, object_type>
      {
        auto&& [column, row, value] = dataItem;
        return { column, row, ConvertToObjectData(value) };
      });
      break;
    case 1:
      std::ranges::transform(image_data { levelData_1 }, inserter, [](auto&& dataItem) -> std::tuple<int, int, object_type>
      {
        auto&& [column, row, value] = dataItem;
        return { column, row, ConvertToObjectData(value) };
      });
      break;
    case 2:
      std::ranges::transform(image_data { levelData_2 }, inserter, [](auto&& dataItem) -> std::tuple<int, int, object_type>
      {
        auto&& [column, row, value] = dataItem;
        return { column, row, ConvertToObjectData(value) };
      });
      break;
  }
}
