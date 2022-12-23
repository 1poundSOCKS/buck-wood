#include "pch.h"
#include "level_state.h"
#include "game_objects.h"
#include "level_render.h"
#include "screen_view.h"
#include "level_object_collisions.h"
#include "level_geometry.h"

const float gameSpeedMultiplier = 2.0f;
const int shotTimeNumerator = 1;
const int shotTimeDenominator = 20;

void UpdatePlayer(level_state& levelState, const level_control_state& controlState);
void UpdateBullets(level_state& levelState, const level_control_state& controlState);
void ProcessCollisions(level_state& levelState);
bullet& GetBullet(std::vector<bullet>& bullets);
bool PlayerCanShoot(const level_state& levelState);
float GetUpdateInterval(const level_state& levelState);
bool BulletHasExpired(const bullet& bullet);
int ProcessBulletTargetCollisions(
  std::vector<bullet_target_collision>::iterator collisionsBegin, 
  std::vector<bullet_target_collision>::iterator collisionsEnd);
auto GenerateLevelBackgroundData(const game_level_data& levelData) -> level_background_data;
[[nodiscard]] auto PlayerHitGround(const level_state& levelState) -> bool;

constexpr D2D1_RECT_F GetStarRect()
{
  return { -4, -4, 4, 4 };
}

inline D2D1_RECT_F GetStarRect(float x, float y)
{
  const D2D1_RECT_F rect = GetStarRect();
  return { rect.left + x, rect.top + y, rect.right + x, rect.bottom + y };
}

level_state::level_state(const game_level_data& levelData, int64_t counterFrequency, const screen_render_data& renderData)
: levelData(levelData), counterFrequency(counterFrequency)
{
  levelTimeLimit = levelData.timeLimitInSeconds * counterFrequency;
  
  shotTimerInterval = ( counterFrequency * shotTimeNumerator ) / shotTimeDenominator;

  player.xPos = levelData.playerStartPosX;
  player.yPos = levelData.playerStartPosY;

  bullets.resize(100);

  std::transform(levelData.targets.cbegin(), levelData.targets.cend(), std::back_inserter(targets), [](const auto& target)
  {
    return target_state(target);
  });

  for( auto& target : targets )
  {
    CreateConnectedLines(target.points.cbegin(), target.points.cend(), std::back_inserter(target.shape));
  }

  level_background_data backgroundData = GenerateLevelBackgroundData(levelData);

  screen_render_brush_selector renderBrushSelector(renderData.renderBrushes);

  auto starBrush = renderBrushSelector[dark_grey];
  std::transform(
    backgroundData.starfield.stars.cbegin(), 
    backgroundData.starfield.stars.cend(),
    std::back_inserter(renderStars),
    [starBrush](auto star) { return render_point { GetStarRect(star.position.x, star.position.y), starBrush }; }
  );

  groundGeometry = CreateLevelGroundGeometry(levelData);
  targetsGeometry = CreateLevelTargetsGeometry(levelData);
}

bool LevelIsComplete(const level_state& levelState)
{
  int activatedTargetCount = std::reduce(
    levelState.targets.cbegin(), 
    levelState.targets.cend(), 
    0, 
    [](auto count, auto& target){ return target.activated ? count + 1 : count; }
  );

  return activatedTargetCount == levelState.targets.size();
}

void UpdateLevelState(level_state& levelState, const level_control_state& controlState, int64_t timerCount)
{
  levelState.previousTimerCount = levelState.currentTimerCount;
  levelState.currentTimerCount = timerCount;

  if( GetPlayTimeRemaining(levelState) > 0 )
  {
    levelState.viewTransform = CreateViewTransform(levelState, controlState.renderTargetMouseData.size, 1.2);
    
    levelState.invertedViewTransform = levelState.viewTransform;
    if( levelState.invertedViewTransform.Invert() )
    {
      D2D1_POINT_2F inPoint { controlState.renderTargetMouseData.x, controlState.renderTargetMouseData.y };
      auto outPoint = levelState.invertedViewTransform.TransformPoint(inPoint);
      levelState.mouseX = outPoint.x;
      levelState.mouseY = outPoint.y;
    }

    UpdatePlayer(levelState, controlState);
    UpdateBullets(levelState, controlState);
    ProcessCollisions(levelState);
  }
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
  UpdateShipGeometryData(levelState.player);
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
}

void ProcessCollisions(level_state& levelState)
{
  if( PlayerHitGround(levelState) || ObjectsHaveCollided(levelState.player, levelState.targetsGeometry) )
    levelState.player.state = player_ship::dead;

  std::vector<bullet_target_collision> bulletTargetCollisions;

  GetBulletTargetCollisions(
    levelState.bullets.begin(), 
    levelState.bullets.end(), 
    levelState.targets.begin(),
    levelState.targets.end(),
    std::back_inserter(bulletTargetCollisions)
  );

  int targetActivatedCount = 
  ProcessBulletTargetCollisions(bulletTargetCollisions.begin(), bulletTargetCollisions.end());
  
  if( targetActivatedCount > 0 )
    levelState.targetShot = true;

  for( auto& bullet : levelState.bullets )
  {
    if( bullet.free || BulletHasExpired(bullet) || CoordinateIsUnderground(bullet.xPos, bullet.yPos, levelState.groundGeometry) )
      bullet.free = true;
  }
}

[[nodiscard]] auto PlayerHitGround(const level_state& levelState) -> bool
{
  return std::reduce(
    levelState.player.points.cbegin(), 
    levelState.player.points.cend(), 
    false, 
    [&levelState](auto hitGround, auto point)
    {
      return hitGround || CoordinateIsUnderground(point.x, point.y, levelState.groundGeometry);
    }
  );
}

bool BulletHasExpired(const bullet& bullet)
{
  float cx = bullet.xPos - bullet.startX;
  float cy = bullet.yPos - bullet.startY;
  float distance = sqrt(cx * cx + cy * cy);
  return distance > bullet.range;
}

int ProcessBulletTargetCollisions(
  std::vector<bullet_target_collision>::iterator collisionsBegin, 
  std::vector<bullet_target_collision>::iterator collisionsEnd)
{
  int activatedCount = 0;

  for( auto& collision = collisionsBegin; collision != collisionsEnd; ++collision )
  {
    collision->bullet.free = true;
    if( !collision->targetState.activated )
    {
      collision->targetState.activated = true;
      ++activatedCount;
    }
  }

  return activatedCount;
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

float GetUpdateInterval(const level_state& levelState)
{
  int64_t intervalCount = levelState.currentTimerCount - levelState.previousTimerCount;
  return static_cast<float>(intervalCount) / static_cast<float>(levelState.counterFrequency) * gameSpeedMultiplier;
}

bool PlayerCanShoot(const level_state& levelState)
{
  return levelState.currentTimerCount - levelState.lastShotTimerValue > levelState.shotTimerInterval;
}

bool PlayerIsDead(const level_state& levelState)
{
  return levelState.player.state == player_ship::dead;
}

bool LevelTimedOut(const level_state& levelState)
{
  return GetPlayTimeRemaining(levelState) > 0 ? false : true;
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

void RemoveObscuredStars(const game_level_data& levelData, auto starsBegin, auto starsEnd, auto starInserter)
{
  auto levelGroundGeometry = CreateLevelGroundGeometry(levelData);
  auto levelTargetsGeometry = CreateLevelTargetsGeometry(levelData);

  std::copy_if(starsBegin, starsEnd, starInserter, [&levelGroundGeometry,&levelTargetsGeometry](auto star)
  {
    return !(
      CoordinateIsUnderground(star.position.x, star.position.y, levelGroundGeometry) || 
      PointInsideObject(star.position.x, star.position.y, levelTargetsGeometry.lines.cbegin(), levelTargetsGeometry.lines.cend())
    );
  });
}

auto GenerateLevelBackgroundData(const game_level_data& levelData) -> level_background_data
{
  level_background_data backgroundData;
  std::vector<level_star> starfield;
  GenerateStarfield(levelData, std::back_inserter(starfield));
  RemoveObscuredStars(levelData, starfield.cbegin(), starfield.cend(), std::back_inserter(backgroundData.starfield.stars));
  return backgroundData;
}
