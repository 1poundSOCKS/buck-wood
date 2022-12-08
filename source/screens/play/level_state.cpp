#include "pch.h"
#include "level_state.h"
#include "game_objects.h"
#include "level_render.h"
#include "screen_view.h"

const float gameSpeedMultiplier = 2.0f;
const int shotTimeNumerator = 1;
const int shotTimeDenominator = 60;

void UpdatePlayer(level_state& levelState, const level_control_state& controlState, const system_timer& timer);
bool PlayerHasHitTheGround(const std::vector<game_point>& player, const std::vector<game_point>& ground);

void UpdateBullets(level_state& levelState, const level_control_state& controlState, const system_timer& timer);
bullet& GetBullet(std::vector<bullet>& bullets);

void UpdatePlayerShipPointData(player_ship_point_data& playerShipPointData, const player_ship& playerShip);

player_ship::player_ship() : xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0)
{
}

bullet::bullet() : range(1000)
{
}

object_state::object_state(const game_level_object_data& data) : data(data)
{
}

target_state::target_state(const game_point& position) : position(position)
{
  std::vector<game_point> pointsTmp;
  CreatePointsForTarget(defaultTargetSize, std::back_inserter(pointsTmp));
  TransformPoints(pointsTmp.cbegin(), pointsTmp.cend(), std::back_inserter(points), 0, position.x, position.y);
}

level_state::level_state(const game_level_data& levelData, const system_timer& systemTimer)
: levelData(levelData), systemTimer(systemTimer), shotTimer(systemTimer, shotTimeNumerator, shotTimeDenominator)
{
  // player
  player.xPos = levelData.playerStartPosX;
  player.yPos = levelData.playerStartPosY;
  UpdatePlayerShipPointData(playerShipPointData, player);

  bullets.resize(100);

  // border
  CreateConnectedLines<game_point>(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend(), std::back_inserter(boundaryLines));

  // objects
  objects.reserve(levelData.objects.size());
  std::transform(levelData.objects.cbegin(), levelData.objects.cend(), std::back_inserter(objects), [](const auto& data)
  {
    return object_state(data);
  });

  for( auto& object : objects)
  {
    CreateConnectedLines<game_point>(object.data.points.cbegin(), object.data.points.cend(), std::back_inserter(object.shape));
  }

  // targets
  targets.reserve(levelData.targets.size());
  std::transform(levelData.targets.cbegin(), levelData.targets.cend(), std::back_inserter(targets), [](const auto& target)
  {
    return target_state(target);
  });

  for( auto& target : targets )
  {
    CreateConnectedLines<game_point>(target.points.cbegin(), target.points.cend(), std::back_inserter(target.shape));
  }

  shotTimer.paused = false;
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

void UpdateLevelState(level_state& levelState, const level_control_state& controlState, const system_timer& timer)
{
  D2D1::Matrix3x2F mouseTransform = CreateViewTransform(levelState, controlState.renderTargetMouseData.size, 1.0);

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

  UpdatePlayerShipPointData(levelState.playerShipPointData, levelState.player);

  const auto& currentLevelData = levelState.levelData;

  if( PlayerHasHitTheGround(levelState.playerShipPointData.transformedPoints, currentLevelData.boundaryPoints) )
  {
    levelState.player.state = player_ship::player_state::state_dead;
    return;
  }

  for( const auto& object : currentLevelData.objects)
  {
    std::vector<game_line> lines;
    CreateConnectedLines<game_point>(object.points.cbegin(), object.points.cend(), std::back_inserter(lines));
    if( AnyPointInside(levelState.playerShipPointData.transformedPoints.cbegin(), levelState.playerShipPointData.transformedPoints.cend(), lines) )
    {
      levelState.player.state = player_ship::player_state::state_dead;
      return;
    }
  }
}

bool PlayerHasHitTheGround(const std::vector<game_point>& player, const std::vector<game_point>& ground)
{
  std::vector<game_line> lines;
  CreateConnectedLines<game_point>(ground.cbegin(), ground.cend(), std::back_inserter(lines), 0, 0, false);

  auto firstPoint = lines.front().start;
  auto lastPoint = lines.back().end;

  std::vector<int> interceptCounts;
  std::transform(
    player.cbegin(), 
    player.cend(), 
    std::back_inserter(interceptCounts),
    [lines,firstPoint,lastPoint](auto& point)
    {
      int count = GetLineInterceptCount(point, lines);
      if( point.x >= firstPoint.x && point.y > firstPoint.y ) count++;
      if( point.x <= lastPoint.x && point.y > lastPoint.y ) count++;
      return count;
    }
  );

  std::vector<bool> pointBelowBoundaryFlags;
  std::transform(
    interceptCounts.cbegin(),
    interceptCounts.cend(),
    std::back_inserter(pointBelowBoundaryFlags),
    [](auto& interceptCount)
    {
      return interceptCount % 2 == 1;
    }
  );

  return std::reduce(
    pointBelowBoundaryFlags.cbegin(),
    pointBelowBoundaryFlags.cend(),
    false,
    [](auto flag, auto below)
    {
      return flag || below;
    }
  );
}

void UpdateBullets(level_state& levelState, const level_control_state& controlState, const system_timer& timer)
{
  float gameUpdateInterval = GetIntervalTimeInSeconds(timer) * gameSpeedMultiplier;

  if( controlState.shoot && GetTicksRemaining(levelState.shotTimer) == 0 )
  {
    static const float bulletSpeed = 200.0f * gameUpdateInterval;
    static const float bulletRange = 2000.0f;

    auto& bullet = GetBullet(levelState.bullets);
    bullet.startX = bullet.xPos = levelState.player.xPos;
    bullet.startY = bullet.yPos = levelState.player.yPos;
    bullet.angle = CalculateAngle(bullet.xPos, bullet.yPos, levelState.mouseX, levelState.mouseY);
    bullet.yVelocity = -bulletSpeed * cos(DEGTORAD(bullet.angle));
    bullet.xVelocity = bulletSpeed * sin(DEGTORAD(bullet.angle));

    levelState.playerShot = true;
    ResetStopwatch(levelState.shotTimer);
    levelState.shotTimer.paused = false;
  }

  for( auto& bullet : levelState.bullets )
  {
    if( bullet.free ) continue;

    bullet.xPos += bullet.xVelocity;
    bullet.yPos += bullet.yVelocity;

    const game_point bulletPoint(bullet.xPos, bullet.yPos);
    bullet.free = !PointInside(bulletPoint, levelState.boundaryLines);

    if( bullet.free ) continue;

    for( const auto& object : levelState.objects )
    {
      if( PointInside(bulletPoint, object.shape) )
      {
        bullet.free = true;
        break;
      }
    }

    if( bullet.free ) continue;

    for( auto& target: levelState.targets )
    {
      if( PointInside(bulletPoint, target.shape) )
      {
        if( !target.activated )
        {
          target.activated = true;
          levelState.targetShot = true;
        }

        bullet.free = true;
        break;
      }
    }

    if( bullet.free ) continue;

    float cx = bullet.xPos - bullet.startX;
    float cy = bullet.yPos - bullet.startY;
    float distance = sqrt(cx * cx + cy * cy);
    if( distance > bullet.range ) bullet.free = true;
  }
}

bullet& GetBullet(std::vector<bullet>& bullets)
{
  for( auto& bullet : bullets )
  {
    if( bullet.free )
    {
      bullet.free = false;
      return bullet;
    }
  }

  return bullets.front();
}

D2D1::Matrix3x2F CreateViewTransform(const level_state& levelState, const D2D1_SIZE_F& renderTargetSize, float renderScale)
{
  return CreateGameLevelTransform(levelState.player.xPos, levelState.player.yPos, renderScale, renderTargetSize.width, renderTargetSize.height);
}

void UpdatePlayerShipPointData(player_ship_point_data& playerShipPointData, const player_ship& playerShip)
{
  playerShipPointData.points.clear();
  playerShipPointData.thrusterPoints.clear();
  playerShipPointData.transformedPoints.clear();
  CreatePointsForPlayer(std::back_inserter(playerShipPointData.points));
  CreatePointsForPlayerThruster(std::back_insert_iterator(playerShipPointData.thrusterPoints));
  TransformPoints(playerShipPointData.points.cbegin(), playerShipPointData.points.cend(), std::back_inserter(playerShipPointData.transformedPoints), playerShip.angle, playerShip.xPos, playerShip.yPos);
}
