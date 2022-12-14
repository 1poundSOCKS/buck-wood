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
void UpdateExplosions(level_state& levelState);
void ProcessCollisions(level_state& levelState);
bullet& GetBullet(std::vector<bullet>& bullets);
bool PlayerCanShoot(const level_state& levelState);
float GetUpdateInterval(const level_state& levelState);
bool BulletHasExpired(const bullet& bullet);
int ProcessBulletTargetCollisions(std::vector<bullet_target_collision>::iterator collisionsBegin, std::vector<bullet_target_collision>::iterator collisionsEnd);
auto GenerateLevelBackgroundData(const game_level_data& levelData) -> level_background_data;
[[nodiscard]] auto PlayerHitGround(const level_state& levelState) -> bool;

void CreateStaticLevelRenderLines(
  const level_state& levelState, 
  std::back_insert_iterator<std::vector<render_line>> insertIterator, 
  screen_render_brush_selector brushes);

constexpr D2D1_RECT_F GetStarRect()
{
  return { -4, -4, 4, 4 };
}

inline D2D1_RECT_F GetStarRect(float x, float y)
{
  const D2D1_RECT_F rect = GetStarRect();
  return { rect.left + x, rect.top + y, rect.right + x, rect.bottom + y };
}

auto GetLevelAreaState(const level_state& levelState, game_rect rect) -> area_state::state_type
{
  auto cornerPoints = std::array {
    game_point { rect.topLeft.x, rect.topLeft.y }, 
    game_point { rect.bottomRight.x, rect.topLeft.y }, 
    game_point { rect.bottomRight.x, rect.bottomRight.y },
    game_point { rect.topLeft.x, rect.bottomRight.y }
  };

  auto IncrementIfUnderground = [&levelState](auto count, auto point) -> int
  {
    return IsUnderground(point.x, point.y, levelState.groundGeometry) ? count + 1 : count;
  };

  auto undergroundCount = std::reduce(cornerPoints.cbegin(), cornerPoints.cend(), 0, IncrementIfUnderground);

  auto pointInsideCount = PointInsideCount(
    levelState.groundGeometry.allPoints.cbegin(),
    levelState.groundGeometry.allPoints.cend(),
    rect);

  switch( undergroundCount )
  {
  case 4:
    return pointInsideCount > 0 ? area_state::split : area_state::keep;
  case 0:
    return pointInsideCount > 0 ? area_state::split : area_state::omit;
  default:
    return area_state::split;
  }
};

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

  CreateStaticLevelRenderLines(*this, std::back_inserter(staticRenderLines), renderBrushSelector);

  auto GetAreaState = [this](game_rect rect) -> area_state::state_type { return GetLevelAreaState(*this, rect); };
  auto levelBoundary = GetLevelBoundary(groundGeometry);
  levelBoundary.topLeft.x -= 800;
  levelBoundary.bottomRight.x += 800;
  levelBoundary.bottomRight.y += 800;
  SplitArea(levelBoundary, 9, std::back_inserter(groundMatrix), GetAreaState);

  level_boundary levelBoundaryObject = { LoadLevelBoundary(levelData) };
  solidObjects.emplace_back(levelBoundaryObject);

  std::vector<game_closed_object> levelObjects;
  LoadLevelObjects(levelData, std::back_inserter(levelObjects));
  std::copy(levelObjects.cbegin(), levelObjects.cend(), std::back_inserter(solidObjects));
}

bool LevelIsComplete(const level_state& levelState)
{
  int activatedTargetCount = std::accumulate(
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

      D2D1_POINT_2F screenTopLeft { 0, 0 };
      D2D1_POINT_2F screenBottomRight { controlState.renderTargetMouseData.size.width - 1, controlState.renderTargetMouseData.size.height - 1 };
      auto viewTopLeft = levelState.invertedViewTransform.TransformPoint(screenTopLeft);
      auto viewBottomRight = levelState.invertedViewTransform.TransformPoint(screenBottomRight);
      levelState.viewRect.topLeft = { viewTopLeft.x, viewTopLeft.y };
      levelState.viewRect.bottomRight = { viewBottomRight.x, viewBottomRight.y };
    }

    if( levelState.player.state == player_ship::alive )
    {
      UpdatePlayer(levelState, controlState);
    }

    UpdateBullets(levelState, controlState);
    UpdateExplosions(levelState);
    ProcessCollisions(levelState);
  }
}

void UpdatePlayer(level_state& levelState, const level_control_state& controlState)
{
  const float forceOfGravity = 20.0f;
  const float playerThrust = 100.0f;

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

  levelState.player.angle = CalculateAngle(
    levelState.player.xPos, levelState.player.yPos, 
    levelState.mouseX, levelState.mouseY);

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

void UpdateExplosions(level_state& levelState)
{
  const float forceOfGravity = 20.0f;
  auto updateInterval = GetUpdateInterval(levelState);

  for( auto& explosion : levelState.explosions )
  {
    UpdateState(explosion, updateInterval, forceOfGravity);
  }
}

void ProcessCollisions(level_state& levelState)
{
  if( levelState.player.state == player_ship::alive )
  {
    if( PlayerHitGround(levelState) || ObjectsHaveCollided(levelState.player, levelState.targetsGeometry) )
    {
      levelState.player.state = player_ship::dead;
      levelState.explosions.emplace_back(CreateExplosion(
        levelState.player.xPos, 
        levelState.player.yPos, 
        levelState.currentTimerCount - levelState.previousTimerCount)
      );
    }
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
    if( !bullet.free )
    {
      auto collisionCount = std::accumulate(levelState.solidObjects.cbegin(), levelState.solidObjects.cend(), 0, 
      [bullet](int count, const solid_object& solidObject) -> int
      {
        return solidObject.HasCollided(bullet.xPos, bullet.yPos) ? count + 1 : count;
      });

      // if( bullet.free || BulletHasExpired(bullet) || IsUnderground(bullet.xPos, bullet.yPos, levelState.groundGeometry) )
      if( BulletHasExpired(bullet) || collisionCount > 0 )
        bullet.free = true;
    }
  }

  for( auto& explosion : levelState.explosions )
  {
    ProcessCollisions(explosion, levelState.groundGeometry);
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
      return hitGround || IsUnderground(point.x, point.y, levelState.groundGeometry);
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
      IsUnderground(star.position.x, star.position.y, levelGroundGeometry) || 
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

void CreateStaticLevelRenderLines(
  const level_state& levelState,
  std::back_insert_iterator<std::vector<render_line>> insertIterator, 
  screen_render_brush_selector brushes)
{
  auto brush = brushes[grey];
  CreateRenderLines(levelState.groundGeometry.allLines.cbegin(), levelState.groundGeometry.allLines.cend(), insertIterator, brush, 5);
}
