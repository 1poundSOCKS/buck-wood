#include "pch.h"
#include "level_state.h"
#include "game_objects.h"
#include "level_render.h"
#include "screen_view.h"
#include "level_object_collisions.h"

const float gameSpeedMultiplier = 2.0f;
const int shotTimeNumerator = 1;
const int shotTimeDenominator = 30;

void UpdatePlayer(level_state& levelState, const level_control_state& controlState);
void UpdateBullets(level_state& levelState, const level_control_state& controlState);
bullet& GetBullet(std::vector<bullet>& bullets);
bool PlayerCanShoot(const level_state& levelState);
void UpdatePlayerShipPointData(player_ship_point_data& playerShipPointData, const player_ship& playerShip);
float GetUpdateInterval(const level_state& levelState);
bool BulletHasExpired(const bullet& bullet);

level_state::level_state(const game_level_data& levelData, int64_t counterFrequency)
: levelData(levelData), counterFrequency(counterFrequency)
{
  levelTimeLimit = levelData.timeLimitInSeconds * counterFrequency;
  
  shotTimerInterval = ( counterFrequency * shotTimeNumerator ) / shotTimeDenominator;

  player.xPos = levelData.playerStartPosX;
  player.yPos = levelData.playerStartPosY;
  UpdatePlayerShipPointData(playerShipPointData, player);

  bullets.resize(100);

  CreateConnectedLines<game_point>(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend(), std::back_inserter(theGround), 0, 0, false);

  std::transform(levelData.objects.cbegin(), levelData.objects.cend(), std::back_inserter(objects), [](const auto& data)
  {
    return object_state(data);
  });

  for( auto& object : objects)
  {
    CreateConnectedLines<game_point>(object.data.points.cbegin(), object.data.points.cend(), std::back_inserter(object.shape));
  }

  std::transform(levelData.targets.cbegin(), levelData.targets.cend(), std::back_inserter(targets), [](const auto& target)
  {
    return target_state(target);
  });

  for( auto& target : targets )
  {
    CreateConnectedLines<game_point>(target.points.cbegin(), target.points.cend(), std::back_inserter(target.shape));
  }
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

void UpdateLevelState(level_state& levelState, const level_control_state& controlState, int64_t timerCount)
{
  levelState.previousTimerCount = levelState.currentTimerCount;
  levelState.currentTimerCount = timerCount;

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

  UpdatePlayer(levelState, controlState);
  UpdateBullets(levelState, controlState);
}

void UpdatePlayer(level_state& levelState, const level_control_state& controlState)
{
  static const float forceOfGravity = 20.0f;
  static const float playerThrust = 100.0f;

  float gameUpdateInterval = GetUpdateInterval(levelState);

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

  if( PlayerHasHitTheGround(levelState.playerShipPointData.transformedPoints, levelState.theGround.cbegin(), levelState.theGround.cend()) )
  {
    levelState.player.state = player_ship::dead;
  }
  else
  {
    for( const auto& object : currentLevelData.objects)
    {
      std::vector<game_line> lines;
      CreateConnectedLines<game_point>(object.points.cbegin(), object.points.cend(), std::back_inserter(lines));
      if( AnyPointInside(levelState.playerShipPointData.transformedPoints.cbegin(), levelState.playerShipPointData.transformedPoints.cend(), lines) )
      {
        levelState.player.state = player_ship::dead;
      }
    }
  }
}

void UpdateBullets(level_state& levelState, const level_control_state& controlState)
{
  float gameUpdateInterval = GetUpdateInterval(levelState);

  if( controlState.shoot && PlayerCanShoot(levelState) )
  {
    static const float bulletSpeed = 200.0f;
    static const float bulletRange = 2000.0f;

    auto& bullet = GetBullet(levelState.bullets);
    bullet.startX = bullet.xPos = levelState.player.xPos;
    bullet.startY = bullet.yPos = levelState.player.yPos;
    bullet.angle = CalculateAngle(bullet.xPos, bullet.yPos, levelState.mouseX, levelState.mouseY);
    bullet.yVelocity = -bulletSpeed * cos(DEGTORAD(bullet.angle));
    bullet.xVelocity = bulletSpeed * sin(DEGTORAD(bullet.angle));
    levelState.lastShotTimerValue = levelState.currentTimerCount;
    levelState.playerShot = true;
  }

  for( auto& bullet : levelState.bullets )
  {
    bullet.xPos += ( bullet.xVelocity * gameUpdateInterval );
    bullet.yPos += ( bullet.yVelocity * gameUpdateInterval );
  }

  std::vector<bullet_target_collision> bulletTargetCollisions;

  GetBulletTargetCollisions(
    levelState.bullets.begin(), 
    levelState.bullets.end(), 
    levelState.targets.begin(),
    levelState.targets.end(),
    std::back_inserter(bulletTargetCollisions)
  );

  int targetActivatedCount = ProcessBulletTargetCollisions(bulletTargetCollisions.begin(), bulletTargetCollisions.end());
  if( targetActivatedCount > 0 )
    levelState.targetShot = true;

  for( auto& bullet : levelState.bullets )
  {
    if( BulletHasExpired(bullet) || BulletHasHitTheGround(bullet, levelState.theGround.cbegin(), levelState.theGround.cend()) )
      bullet.free = true;
  }
}

bool BulletHitObject(const level_state& levelState, const bullet& bullet)
{
  const game_point bulletPoint(bullet.xPos, bullet.yPos);

  for( const auto& object : levelState.objects )
  {
    if( PointInside(bulletPoint, object.shape) ) return true;
  }

  return false;
}


bool BulletHasExpired(const bullet& bullet)
{
  float cx = bullet.xPos - bullet.startX;
  float cy = bullet.yPos - bullet.startY;
  float distance = sqrt(cx * cx + cy * cy);
  return distance > bullet.range;
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

float GetUpdateInterval(const level_state& levelState)
{
  int64_t intervalCount = levelState.currentTimerCount - levelState.previousTimerCount;
  return static_cast<float>(intervalCount) / static_cast<float>(levelState.counterFrequency) * gameSpeedMultiplier;
}

bool PlayerCanShoot(const level_state& levelState)
{
  return levelState.currentTimerCount - levelState.lastShotTimerValue > levelState.shotTimerInterval;
}

int64_t GetPlayTimeRemaining(const level_state& screenState)
{
  int64_t playTimeRemaining = screenState.levelTimeLimit - screenState.currentTimerCount;
  return max(0, playTimeRemaining);
}

float GetPlayTimeRemainingInSeconds(const level_state& screenState)
{
  return static_cast<float>(GetPlayTimeRemaining(screenState)) / static_cast<float>(screenState.counterFrequency);
}
