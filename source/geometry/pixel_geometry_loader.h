#pragma once

#include "pixel_geometry.h"

namespace pixel_geometry_loader
{

  [[nodiscard]] auto read(std::ranges::input_range auto&& pixelData, cell_size pixelSize, auto&& pixelCheckFunction) -> pixel_geometry;

};

auto pixel_geometry_loader::read(std::ranges::input_range auto &&pixelData, cell_size pixelSize, auto &&pixelCheckFunction) -> pixel_geometry
{
  // std::list<cell_id> playerPixelData;

  // for( const auto& pixelInfo : pixelData )
  // {
  //   const auto& [column, row, pixelData] = pixelInfo;
  //   if( pixelCheckFunction(pixelData) ) playerPixelData.emplace_back(column, row);
  // });

  // pixel_geometry playerPixelGeometry;
  // playerPixelGeometry.Load(playerPixelData, pixelSize);
  // return playerPixelGeometry;

  level_base playerPixelImageReader { pixelData };

  std::list<cell_id> playerPixelData;

  playerPixelImageReader.Enumerate([&playerPixelData,&pixelCheckFunction](int column, int row, char pixelData)
  {
    if( pixelCheckFunction(pixelData) ) playerPixelData.emplace_back(column, row);
  });

  pixel_geometry playerPixelGeometry;
  playerPixelGeometry.Load(playerPixelData, pixelSize);
  return playerPixelGeometry;
}
