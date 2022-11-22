#include "pch.h"
#include "level_state.h"
#include "game_objects.h"

const float gameSpeedMultiplier = 2.0f;
const int shotTimeNumerator = 1;
const int shotTimeDenominator = 60;

void UpdatePlayer(level_state& levelState, const level_control_state& controlState, const system_timer& timer);
void UpdateBullets(level_state& levelState, const level_control_state& controlState, const system_timer& timer);
D2D1::Matrix3x2F CreateViewTransform(const level_state& levelState, const D2D1_SIZE_F& renderTargetSize);

player_ship::player_ship() : xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0)
{
}

bullet::bullet(float x, float y, float range) : startX(x), startY(y), xPos(x), yPos(y), range(range), xVelocity(0), yVelocity(0), angle(0), outsideLevel(false)
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
  player.xPos = levelData.playerStartPosX;
  player.yPos = levelData.playerStartPosY;
  CreatePointsForPlayer(std::back_inserter(player.points));
  CreatePointsForPlayerThruster(std::back_insert_iterator(player.thrusterPoints));

  // border
  CreateConnectedLines<game_point>(levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend(), std::back_inserter(boundaryLines));

  // objects
  objectShapes.resize(levelData.objects.size());
  int objectIndex = 0;

  for( const auto& object : levelData.objects)
  {
    CreateConnectedLines<game_point>(object.points.cbegin(), object.points.cend(), std::back_inserter(objectShapes[objectIndex++]));
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

  // targetShapes.resize(targets.size());
  // int targetIndex = 0;

  // for( auto& target: targets )
  // {
  //   CreateConnectedLines<game_point>(target.points.cbegin(), target.points.cend(), std::back_inserter(targetShapes[targetIndex++]));
  // }

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

  levelState.player.transformedPoints.clear();
  TransformPoints(levelState.player.points.cbegin(), levelState.player.points.cend(), std::back_inserter(levelState.player.transformedPoints), levelState.player.angle, levelState.player.xPos, levelState.player.yPos);

  const auto& currentLevelData = levelState.levelData;

  std::vector<game_line> lines;
  CreateConnectedLines<game_point>(currentLevelData.boundaryPoints.cbegin(), currentLevelData.boundaryPoints.cend(), std::back_inserter(lines));

  if( !AllPointsInside(levelState.player.transformedPoints.cbegin(), levelState.player.transformedPoints.cend(), lines) )
  {
    levelState.player.state = player_ship::player_state::state_dead;
    return;
  }
  
  for( const auto& object : currentLevelData.objects)
  {
    std::vector<game_line> lines;
    CreateConnectedLines<game_point>(object.points.cbegin(), object.points.cend(), std::back_inserter(lines));
    if( AnyPointInside(levelState.player.transformedPoints.cbegin(), levelState.player.transformedPoints.cend(), lines) )
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
    bullet->outsideLevel = !PointInside(bulletPoint, levelState.boundaryLines);

    for( const auto& objectShape : levelState.objectShapes )
    {
      if( PointInside(bulletPoint, objectShape) ) bullet->outsideLevel = true;
    }

    for( auto& target: levelState.targets )
    {
      if( PointInside(bulletPoint, target.shape) )
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

void RenderFrame(const d2d_frame& frame, const level_state& levelState, const render_brushes& brushes)
{
  auto renderTargetSize = frame.renderTarget->GetSize();
  frame.renderTarget->SetTransform(levelState.viewTransform);

  std::vector<render_line> renderLines;
  CreateRenderLines(levelState, std::back_inserter(renderLines));
  RenderLines(frame.renderTarget, brushes, 2, renderLines.cbegin(), renderLines.cend());

  for( const auto& bullet : levelState.bullets )
  {
    RenderBullet(frame.renderTarget, levelState.viewTransform, *bullet, brushes);
  }
}

void RenderBullet(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const D2D1::Matrix3x2F& viewTransform, const bullet& bullet, const render_brushes& brushes)
{
  static const float bulletSize = 3.0f;

  auto translate = D2D1::Matrix3x2F::Translation(bullet.xPos, bullet.yPos);
  auto transform = translate * viewTransform;
  renderTarget->SetTransform(transform);
  auto rectangle = D2D1::RectF(- bulletSize / 2, - bulletSize / 2, bulletSize / 2, bulletSize / 2);
  renderTarget->FillRectangle(&rectangle, brushes.brushRed.get());
}

void CreateRenderLines(const level_state& levelState, std::back_insert_iterator<std::vector<render_line>> renderLines)
{
  CreateRenderLines(levelState.levelData, renderLines);

  for( const auto& target : levelState.targets )
  {
    auto brushColor = target.activated ? render_brushes::color::color_red : render_brushes::color::color_green;
    std::vector<game_point> points;
    CreatePointsForTarget(defaultTargetSize, std::back_inserter(points));
    std::vector<game_point> transformedPoints;
    TransformPoints(points.cbegin(), points.cend(), std::back_inserter(transformedPoints), 0, target.position.x, target.position.y);
    CreateConnectedRenderLines<game_point>(transformedPoints.cbegin(), transformedPoints.cend(), renderLines, brushColor);
  }

  const auto& player = levelState.player;
  CreateConnectedRenderLines<game_point>(player.transformedPoints.cbegin(), player.transformedPoints.cend(), renderLines, render_brushes::color::color_white);

  if( levelState.player.thrusterOn )
  {
    std::vector<game_point> points;
    points.reserve(player.thrusterPoints.size());
    TransformPoints(player.thrusterPoints.cbegin(), player.thrusterPoints.cend(), std::back_inserter(points), player.angle, player.xPos, player.yPos);
    CreateDisconnectedRenderLines<game_point>(points.cbegin(), points.cend(), renderLines, render_brushes::color::color_red);
  }
}

D2D1::Matrix3x2F CreateViewTransform(const level_state& levelState, const D2D1_SIZE_F& renderTargetSize)
{
  static const float renderScale = 1.0f;
  return CreateGameLevelTransform(levelState.player.xPos, levelState.player.yPos, renderScale, renderTargetSize.width, renderTargetSize.height);
}
