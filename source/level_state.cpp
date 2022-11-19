#include "level_state.h"
#include "game_objects.h"

level_state::level_state(const game_level_data& levelData) : levelData(levelData)
{
  player.xPos = levelData.playerStartPosX;
  player.yPos = levelData.playerStartPosY;

  std::transform(levelData.targets.cbegin(), levelData.targets.cend(), std::back_inserter(targets), [](const auto& target)
  {
    return target_state(target);
  });
}

bool LevelIsComplete(const level_state& levelState)
{
  int activatedTargetCount = 0;
  for( const auto& target: levelState.targets )
  {
    if( target.activated ) activatedTargetCount++;
  }

  return activatedTargetCount == levelState.targets.size();
}

void CreateRenderLines(const level_state& levelState, std::back_insert_iterator<std::vector<render_line>> renderLines)
{
  CreateRenderLines(levelState.levelData, renderLines);

  for( const auto& target : levelState.targets )
  {
    render_brushes::color brushColor = target.activated ? render_brushes::color::color_red : render_brushes::color::color_green;
    std::vector<game_point> points;
    CreatePointsForTarget(target.position.x, target.position.y, 40, std::back_inserter(points));
    CreateConnectedRenderLines<game_point>(points.cbegin(), points.cend(), renderLines, render_brushes::color::color_green);
  }

  std::vector<game_point> transformedPoints;
  CreatePointsForPlayer(levelState.player.xPos, levelState.player.yPos, levelState.player.angle, std::back_insert_iterator(transformedPoints));
  CreateConnectedRenderLines<game_point>(transformedPoints.cbegin(), transformedPoints.cend(), renderLines, render_brushes::color::color_white);

  if( levelState.player.thrusterOn )
  {
    std::vector<game_point> transformedPoints;
    CreatePointsForPlayerThruster(levelState.player.xPos, levelState.player.yPos, levelState.player.angle, std::back_insert_iterator(transformedPoints));
    CreateDisconnectedRenderLines<game_point>(transformedPoints.cbegin(), transformedPoints.cend(), renderLines, render_brushes::color::color_red);
  }
}
