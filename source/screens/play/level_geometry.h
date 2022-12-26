#ifndef _level_geometry_
#define _level_geometry_

#include "game_level_data.h"

struct level_ground_geometry
{
  game_rect boundary;
  game_point groundStart;
  game_point groundEnd;
  std::vector<game_line> lines;
};

struct level_targets_geometry
{
  std::vector<game_point> points;
  std::vector<game_line> lines;
};

[[nodiscard]] auto CreateLevelGroundGeometry(const game_level_data& levelData) -> level_ground_geometry [[nothrow]];
[[nodiscard]] auto CreateLevelTargetsGeometry(const game_level_data& levelData) -> level_targets_geometry [[nothrow]];
[[nodiscard]] auto IsUnderground(float x, float y, const level_ground_geometry& levelGroundGeometry) -> bool [[nothrow]];

namespace level_grid
{
  struct definition
  {
    int columnWidth;
    int rowHeight;
    int firstColumn;
    int columnCount;
    int firstRow;
    int rowCount;
  };

  struct cell_ref
  {
    int column;
    int row;
  };

  struct area_state
  {
    enum state_type { not_underground, part_underground, all_underground };
    game_rect rect;
    state_type state;
  };

  [[nodiscard]] auto GetDefinition(int columnWidth, int rowHeight, const level_ground_geometry& groundGeometry) -> definition [[nothrow]];
  [[nodiscard]] auto GetDefinition(int scaleFactor, const level_ground_geometry& groundGeometry) -> definition [[nothrow]];
  [[nodiscard]] auto CreateCellReferences(const definition& gridDef) -> std::vector<cell_ref> [[nothrow]];
  [[nodiscard]] auto CreateGrid(definition gridDef, const std::vector<cell_ref>& cellRefs) -> std::vector<game_rect> [[nothrow]];
  [[nodiscard]] auto GetUndergroundState(game_rect rect, const level_ground_geometry& groundGeometry) -> area_state::state_type [[nothrow]];
  [[nodiscard]] auto CreateMatrix(const std::vector<game_rect>& grid, const level_ground_geometry& groundGeometry) -> std::vector<area_state> [[nothrow]];
  [[nodiscard]] auto SplitMatrixPartials(const std::vector<area_state>& matrix, const level_ground_geometry& groundGeometry) -> std::vector<area_state> [[nothrow]];
};

#endif
