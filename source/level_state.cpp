#include "pch.h"
#include "level_state.h"
#include "game_objects.h"
#include "level_render.h"

const float gameSpeedMultiplier = 2.0f;
const int shotTimeNumerator = 1;
const int shotTimeDenominator = 60;

void UpdatePlayer(level_state& levelState, const level_control_state& controlState, const system_timer& timer);
void UpdateBullets(level_state& levelState, const level_control_state& controlState, const system_timer& timer);
bullet& GetBullet(std::vector<bullet>& bullets);
D2D1::Matrix3x2F CreateViewTransform(const level_state& levelState, const D2D1_SIZE_F& renderTargetSize);
void UpdatePlayerShipPointData(player_ship_point_data& playerShipPointData, const player_ship& playerShip);
void CreateStaticLevelRenderLines(const game_level_data& gameLevelData, std::back_insert_iterator<std::vector<render_line>> insertIterator, screen_render_brush_selector brushes);
void CreateDynamicLevelRenderLines(const level_state& levelState, std::back_insert_iterator<std::vector<render_line>> renderLines, screen_render_brush_selector brushes);

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

  // CreateRenderLines(levelData, std::back_inserter(renderData.staticRenderLines));

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
  levelState.viewTransform = CreateViewTransform(levelState, controlState.renderTargetMouseData.size);

  D2D1::Matrix3x2F mouseTransform = levelState.viewTransform;

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

  // levelState.renderData.renderLines.clear();
  // CreateRenderLines(levelState, std::back_inserter(levelState.renderData.renderLines));
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

  std::vector<game_line> lines;
  CreateConnectedLines<game_point>(currentLevelData.boundaryPoints.cbegin(), currentLevelData.boundaryPoints.cend(), std::back_inserter(lines));

  if( !AllPointsInside(levelState.playerShipPointData.transformedPoints.cbegin(), levelState.playerShipPointData.transformedPoints.cend(), lines) )
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

void RenderFrame(
  ID2D1RenderTarget* renderTarget, 
  screen_render_brush_selector renderBrushSelector, 
  const level_state& levelState)
{
  auto renderTargetSize = renderTarget->GetSize();
  renderTarget->SetTransform(levelState.viewTransform);

  std::vector<render_line> staticRenderLines;
  CreateStaticLevelRenderLines(levelState.levelData, std::back_inserter(staticRenderLines), renderBrushSelector);
  RenderLines(renderTarget, staticRenderLines.cbegin(), staticRenderLines.cend());

  std::vector<render_line> renderLines;
  CreateDynamicLevelRenderLines(levelState, std::back_inserter(renderLines), renderBrushSelector);
  RenderLines(renderTarget, renderLines.cbegin(), renderLines.cend());

  std::vector<render_point> renderBullets;
  for( const auto& bullet : levelState.bullets )
  {
    if( bullet.free ) continue;
    renderBullets.emplace_back(render_point(bullet.xPos, bullet.yPos, renderBrushSelector[red], 8));
  }

  RenderPoints(renderTarget, renderBullets.cbegin(), renderBullets.cend());
}

void CreateStaticLevelRenderLines(const game_level_data& gameLevelData, std::back_insert_iterator<std::vector<render_line>> insertIterator, screen_render_brush_selector brushes)
{
  CreateConnectedRenderLines(gameLevelData.boundaryPoints.cbegin(), gameLevelData.boundaryPoints.cend(), insertIterator, brushes[grey], 6);

  for( const auto& object : gameLevelData.objects )
  {
    CreateConnectedRenderLines(object.points.cbegin(), object.points.cend(), insertIterator, brushes[grey], 6);
  }
}

void CreateDynamicLevelRenderLines(const level_state& levelState, std::back_insert_iterator<std::vector<render_line>> renderLines, screen_render_brush_selector brushes)
{
  for( const auto& target : levelState.targets )
  {
    auto renderBrush = target.activated ? brushes[red] : brushes[green];
    std::vector<game_point> points;
    CreatePointsForTarget(defaultTargetSize, std::back_inserter(points));
    std::vector<game_point> transformedPoints;
    TransformPoints(points.cbegin(), points.cend(), std::back_inserter(transformedPoints), 0, target.position.x, target.position.y);
    CreateConnectedRenderLines(transformedPoints.cbegin(), transformedPoints.cend(), renderLines, renderBrush, 4);
  }

  const auto& player = levelState.player;
  CreateConnectedRenderLines(
    levelState.playerShipPointData.transformedPoints.cbegin(), 
    levelState.playerShipPointData.transformedPoints.cend(), 
    renderLines, 
    brushes[white], 
    2);

  if( levelState.player.thrusterOn )
  {
    std::vector<game_point> points;
    points.reserve(levelState.playerShipPointData.thrusterPoints.size());
    
    TransformPoints(
      levelState.playerShipPointData.thrusterPoints.cbegin(), 
      levelState.playerShipPointData.thrusterPoints.cend(), 
      std::back_inserter(points), 
      player.angle, 
      player.xPos, 
      player.yPos);

    CreateDisconnectedRenderLines(points.cbegin(), points.cend(), renderLines, brushes[red], 5);
  }
}

D2D1::Matrix3x2F CreateViewTransform(const level_state& levelState, const D2D1_SIZE_F& renderTargetSize)
{
  static const float renderScale = 1.0f;
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
