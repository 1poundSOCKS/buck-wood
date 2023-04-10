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

void UpdateExplosions(level_state& levelState);
bullet& GetBullet(std::vector<bullet>& bullets);
float GetUpdateInterval(const level_state& levelState);
bool BulletHasExpired(const bullet& bullet);

constexpr D2D1_RECT_F GetStarRect()
{
  return { -4, -4, 4, 4 };
}

inline D2D1_RECT_F GetStarRect(float x, float y)
{
  const D2D1_RECT_F rect = GetStarRect();
  return { rect.left + x, rect.top + y, rect.right + x, rect.bottom + y };
}

level_state::level_state(const game_level_data& levelData, int64_t counterFrequency, const screen_render_data& renderData) : 
  levelData(levelData), counterFrequency(counterFrequency), 
  controlState(std::make_shared<player_control_state>()),
  brushes { renderData.renderBrushes }
{
  levelTimeLimit = levelData.timeLimitInSeconds * counterFrequency;
  
  shotTimerInterval = ( counterFrequency * shotTimeNumerator ) / shotTimeDenominator;

  std::vector<game_closed_object> levelObjects;
  LoadLevelObjects(levelData, std::back_inserter(levelObjects));

  std::vector<level_island> islands;
  std::transform(levelObjects.cbegin(), levelObjects.cend(), std::back_inserter(islands), [this](const auto& object) -> level_island
  {
    return { object, this->brushes };
  });
  
  std::copy(islands.cbegin(), islands.cend(), std::back_inserter(solidObjects));
  
  player_ship player(screen_render_brush_selector(renderData.renderBrushes));

  playerData = player.data;
  playerData->xPos = levelData.playerStartPosX;
  playerData->yPos = levelData.playerStartPosY;
  playerData->controlState = controlState;

  playerData->eventShot = [this](bullet newBullet) -> void
  {
    solidObjects.push_back(newBullet);
  };

  solidObjects.push_back(player);

  std::vector<target_state> targets;
  std::transform(levelData.targets.cbegin(), levelData.targets.cend(), std::back_inserter(targets), [this](const auto& target)
  {
    return target_state(target, this->brushes);
  });

  std::copy(targets.cbegin(), targets.cend(), std::back_inserter(solidObjects));
}

bool LevelIsComplete(const level_state& levelState)
{
  int total = std::reduce(
    levelState.solidObjects.cbegin(), 
    levelState.solidObjects.cend(), 
    0, 
    [](auto count, const solid_object& object){ return object.LevelIsComplete() ? count + 1 : count; }
  );

  return total == levelState.solidObjects.size();
}

void UpdateLevelState(level_state& levelState, const level_control_state& controlState, int64_t timerCount)
{
  levelState.controlState->thrust = controlState.thrust;
  levelState.controlState->shoot = controlState.shoot;
  
  levelState.previousTimerCount = levelState.currentTimerCount;
  levelState.currentTimerCount = timerCount;

  if( GetPlayTimeRemaining(levelState) > 0 )
  {
    std::list<play_event> events;

    std::for_each(std::execution::seq, levelState.solidObjects.begin(), levelState.solidObjects.end(), [&levelState, timerCount, &events](auto& object)
    {
      object.Update(levelState.counterFrequency, timerCount - levelState.previousTimerCount, std::back_inserter(events));
    });

    std::for_each(events.cbegin(), events.cend(), [](auto& event)
    {
      event.Trigger();
    });

    std::for_each(std::execution::seq, levelState.solidObjects.begin(), levelState.solidObjects.end(), [&levelState, timerCount, &events](auto& mainObject)
    {
      std::for_each(levelState.solidObjects.begin(), levelState.solidObjects.end(), [&mainObject, &events](auto& collisionObject)
      {
        auto collisionData = collisionObject.GetCollisionData();
        if( mainObject.HasCollidedWith(collisionData) )
        {
          auto effect = collisionObject.GetCollisionEffect();
          mainObject.ApplyCollisionEffect(effect);
        }
      });
    });

    auto object = levelState.solidObjects.begin();

    while( object != levelState.solidObjects.end() )
    {
      object = object->Destroyed() ? levelState.solidObjects.erase(object) : ++object;
    }

    levelState.viewTransform = CreateViewTransform(levelState, controlState.renderTargetMouseData.size, 1.2);
    
    levelState.invertedViewTransform = levelState.viewTransform;
    if( levelState.invertedViewTransform.Invert() )
    {
      D2D1_POINT_2F inPoint { controlState.renderTargetMouseData.x, controlState.renderTargetMouseData.y };
      auto outPoint = levelState.invertedViewTransform.TransformPoint(inPoint);
      levelState.controlState->mouseX = levelState.mouseX = outPoint.x;
      levelState.controlState->mouseY = levelState.mouseY = outPoint.y;

      D2D1_POINT_2F screenTopLeft { 0, 0 };
      D2D1_POINT_2F screenBottomRight { controlState.renderTargetMouseData.size.width - 1, controlState.renderTargetMouseData.size.height - 1 };
      auto viewTopLeft = levelState.invertedViewTransform.TransformPoint(screenTopLeft);
      auto viewBottomRight = levelState.invertedViewTransform.TransformPoint(screenBottomRight);
      levelState.viewRect.topLeft = { viewTopLeft.x, viewTopLeft.y };
      levelState.viewRect.bottomRight = { viewBottomRight.x, viewBottomRight.y };
    }
  }
}

bool BulletHasExpired(const bullet& bullet)
{
  float cx = bullet.xPos - bullet.startX;
  float cy = bullet.yPos - bullet.startY;
  float distance = sqrt(cx * cx + cy * cy);
  return distance > bullet.range;
}

D2D1::Matrix3x2F CreateViewTransform(const level_state& levelState, const D2D1_SIZE_F& renderTargetSize, float renderScale)
{
  return CreateGameLevelTransform(levelState.playerData->xPos, levelState.playerData->yPos, renderScale, renderTargetSize.width, renderTargetSize.height);
}

float GetUpdateInterval(const level_state& levelState)
{
  int64_t intervalCount = levelState.currentTimerCount - levelState.previousTimerCount;
  return static_cast<float>(intervalCount) / static_cast<float>(levelState.counterFrequency) * gameSpeedMultiplier;
}

bool PlayerIsDead(const level_state& levelState)
{
  return levelState.playerData->state == player_ship::dead;
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
