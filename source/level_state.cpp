#include "level_state.h"
#include "game_math.h"

const float gameSpeedMultiplier = 2.0f;
const int shotTimeNumerator = 1;
const int shotTimeDenominator = 60;

void UpdatePlayer(level_state& levelState, const level_control_state& controlState, const system_timer& timer);
void UpdateBullets(level_state& levelState, const level_control_state& controlState, const system_timer& timer);
// bool BulletHasExpired(const std::unique_ptr<bullet>& bullet);

level_state::level_state(const game_level_data& levelData, const system_timer& systemTimer)
: levelData(levelData), systemTimer(systemTimer), shotTimer(systemTimer, shotTimeNumerator, shotTimeDenominator)
{
  player.xPos = levelData.playerStartPosX;
  player.yPos = levelData.playerStartPosY;

  std::transform(levelData.targets.cbegin(), levelData.targets.cend(), std::back_inserter(targets), [](const auto& target)
  {
    return target_state(target);
  });

  shotTimer.paused = false;
}

void RefreshControlState(level_control_state& controlState, const control_state& baseControlState)
{
  controlState.shoot = baseControlState.leftMouseButtonDown;
  controlState.thrust = baseControlState.rightMouseButtonDown;
  controlState.renderTargetMouseData = baseControlState.renderTargetMouseData;
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

void UpdateState(level_state& levelState, const level_control_state& controlState, const system_timer& timer)
{
  D2D1::Matrix3x2F mouseTransform = CreateViewTransform(levelState, controlState.renderTargetMouseData.size);

  if( mouseTransform.Invert() )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = controlState.renderTargetMouseData.x;
    inPoint.y = controlState.renderTargetMouseData.y;
    auto outPoint = mouseTransform.TransformPoint(inPoint);
    levelState.mouseX = outPoint.x;
    levelState.mouseY = outPoint.y;
  }

  UpdatePlayer(levelState, controlState, timer);
  UpdateBullets(levelState, controlState, timer);
}

void UpdatePlayer(level_state& levelState, const level_control_state& controlState, const system_timer& timer)
{
  static const float forceOfGravity = 20.0f;
  static const float playerThrust = 100.0f;

  float gameUpdateInterval = GetIntervalTimeInSeconds(timer) * gameSpeedMultiplier;

  float forceX = 0.0f;
  float forceY = forceOfGravity;

  if( controlState.thrust )
  {
    levelState.player.thrusterOn = true;
    forceX += playerThrust * sin(DEGTORAD(levelState.player.angle));
    forceY -= playerThrust * cos(DEGTORAD(levelState.player.angle));
  }
  else
  {
    levelState.player.thrusterOn = false;
  }
  
  levelState.player.xVelocity += forceX * gameUpdateInterval;
  levelState.player.yVelocity += forceY * gameUpdateInterval;
  levelState.player.xPos += levelState.player.xVelocity * gameUpdateInterval;
  levelState.player.yPos += levelState.player.yVelocity * gameUpdateInterval;
  levelState.player.angle = CalculateAngle(levelState.player.xPos, levelState.player.yPos, levelState.mouseX, levelState.mouseY);

  std::vector<game_point> player;
  CreatePointsForPlayer(levelState.player.xPos, levelState.player.yPos, levelState.player.angle, std::back_inserter(player));

  const auto& currentLevelData = levelState.levelData;

  std::vector<game_line> lines;
  CreateConnectedLines<game_point>(currentLevelData.boundaryPoints.cbegin(), currentLevelData.boundaryPoints.cend(), std::back_inserter(lines));
  if( !PointInside(player.cbegin(), player.cend(), lines) )
  {
    levelState.player.state = player_ship::player_state::state_dead;
    return;
  }
  
  for( const auto& object : currentLevelData.objects)
  {
    std::vector<game_line> lines;
    CreateConnectedLines<game_point>(object.points.cbegin(), object.points.cend(), std::back_inserter(lines));
    if( PointsInside(player.cbegin(), player.cend(), lines) )
    {
      levelState.player.state = player_ship::player_state::state_dead;
      return;
    }
  }
}

void UpdateBullets(level_state& levelState, const level_control_state& controlState, const system_timer& timer)
{
  float gameUpdateInterval = GetIntervalTimeInSeconds(timer) * gameSpeedMultiplier;

  if( controlState.shoot && GetTicksRemaining(levelState.shotTimer) == 0 )
  {
    static const float bulletSpeed = 200.0f * gameUpdateInterval;
    static const float bulletRange = 2000.0f;

    std::unique_ptr<bullet> newBullet = std::make_unique<bullet>(levelState.player.xPos, levelState.player.yPos, bulletRange);
    float angle = CalculateAngle(levelState.player.xPos, levelState.player.yPos, levelState.mouseX, levelState.mouseY);
    newBullet->angle = angle;
    newBullet->yVelocity = -bulletSpeed * cos(DEGTORAD(angle));
    newBullet->xVelocity = bulletSpeed * sin(DEGTORAD(angle));
    levelState.bullets.push_front(std::move(newBullet));

    levelState.playerShot = true;
    ResetStopwatch(levelState.shotTimer);
    levelState.shotTimer.paused = false;
  }

  const auto& levelData = levelState.levelData;

  for(const auto& bullet : levelState.bullets)
  {
    bullet->xPos += bullet->xVelocity;
    bullet->yPos += bullet->yVelocity;

    const game_point bulletPoint(bullet->xPos, bullet->yPos);
    std::vector<game_line> lines;
    CreateConnectedLines<game_point>(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend(), std::back_inserter(lines));
    bullet->outsideLevel = !PointInside(bulletPoint, lines);

    for( const auto& object : levelData.objects)
    {
      std::vector<game_line> lines;
      CreateConnectedLines<game_point>(object.points.cbegin(), object.points.cend(), std::back_inserter(lines));
      if( PointInside(bulletPoint, lines) ) bullet->outsideLevel = true;
    }

    for( auto& target: levelState.targets )
    {
      std::vector<game_line> lines;
      CreateConnectedLines<game_point>(target.points.cbegin(), target.points.cend(), std::back_inserter(lines));

      if( PointInside(bulletPoint, lines) )
      {
        bullet->outsideLevel = true;

        if( !target.activated )
        {
          target.activated = true;
          levelState.targetShot = true;
        }
      }
    }
  }

  levelState.bullets.remove_if([](const auto& bullet)
  {
    float cx = bullet->xPos - bullet->startX;
    float cy = bullet->yPos - bullet->startY;
    float distance = sqrt(cx * cx + cy * cy);
    return distance > bullet->range || bullet->outsideLevel;
  });
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

D2D1::Matrix3x2F CreateViewTransform(const level_state& levelState, const D2D1_SIZE_F& renderTargetSize)
{
  static const float renderScale = 1.0f;
  return CreateGameLevelTransform(levelState.player.xPos, levelState.player.yPos, renderScale, renderTargetSize.width, renderTargetSize.height);
}
