#pragma once

#include "geometry/pixel_geometry_loader.h"

namespace level_data
{

  auto Load(int index, auto inserter) -> bool;

};

auto level_data::Load(int index, auto inserter) -> bool
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
      pixel_geometry_loader::imageDataToPixelData(levelData, inserter);
      return true;
      
    default:
      return false;
  }
}
