#include "pch.h"
#include "level_state.h"
#include "game_objects.h"
#include "level_render.h"
#include "screen_view.h"
#include "game_constants.h"

level_state::level_state(const game_level_data& levelData, int64_t counterFrequency, const screen_render_data& renderData) : 
  levelData(levelData), counterFrequency(counterFrequency), 
  m_controlState(std::make_shared<player_control_state>()),
  brushes { renderData.renderBrushes }
{
  levelTimeLimit = levelData.timeLimitInSeconds * counterFrequency;
  
  std::vector<game_closed_object> levelObjects;
  LoadLevelObjects(levelData, std::back_inserter(levelObjects));

  std::vector<level_island> islands;
  std::transform(levelObjects.cbegin(), levelObjects.cend(), std::back_inserter(islands), [this](const auto& object) -> level_island
  {
    return { object, this->brushes };
  });
  
  std::copy(islands.cbegin(), islands.cend(), std::back_inserter(m_activeObjects));
  
  player_ship player(counterFrequency, screen_render_brush_selector(renderData.renderBrushes));

  playerData = player.data;
  playerData->xPos = levelData.playerStartPosX;
  playerData->yPos = levelData.playerStartPosY;
  playerData->controlState = m_controlState;

  playerData->eventShot = [this](bullet newBullet) -> void
  {
    m_activeObjects.push_back(newBullet);
  };

  m_activeObjects.push_back(player);

  std::vector<target_state> targets;
  std::transform(levelData.targets.cbegin(), levelData.targets.cend(), std::back_inserter(targets), [this](const auto& target)
  {
    return target_state(target, this->brushes);
  });

  std::copy(targets.cbegin(), targets.cend(), std::back_inserter(m_activeObjects));
}

[[nodiscard]] auto level_state::IsComplete() -> bool
{
  int total = std::reduce(m_activeObjects.cbegin(), m_activeObjects.cend(), 0, [](auto count, const active_object& object)
  {
    return object.LevelIsComplete() ? count + 1 : count;
  });

  return total == m_activeObjects.size();
}

auto level_state::Update(const level_control_state& levelControlState, int64_t counterValue) -> void
{
  playerShot = targetShot = false;

  m_controlState->thrust = levelControlState.thrust;
  m_controlState->shoot = levelControlState.shoot;
  
  previousTimerCount = currentTimerCount;
  currentTimerCount = counterValue;

  if( GetPlayTimeRemaining() > 0 )
  {
    std::list<play_event> events;
    std::for_each(std::execution::seq, m_activeObjects.begin(), m_activeObjects.end(), [this, &events](auto& object)
    {
      object.Update(this->counterFrequency, this->currentTimerCount - this->previousTimerCount, std::back_inserter(events));
    });

    std::for_each(events.cbegin(), events.cend(), [](auto& event)
    {
      event.Trigger();
    });

    std::for_each(std::execution::seq, m_activeObjects.begin(), m_activeObjects.end(), [this, &events](auto& mainObject)
    {
      std::for_each(m_activeObjects.begin(), m_activeObjects.end(), [&mainObject, &events](auto& collisionObject)
      {
        auto collisionData = collisionObject.GetCollisionData();
        if( mainObject.HasCollidedWith(collisionData) )
        {
          auto effect = collisionObject.GetCollisionEffect();
          mainObject.ApplyCollisionEffect(effect);
        }
      });
    });

    auto object = m_activeObjects.begin();
    while( object != m_activeObjects.end() )
    {
      object = object->Destroyed() ? m_activeObjects.erase(object) : ++object;
    }

    viewTransform = CreateViewTransform(levelControlState.renderTargetMouseData.size, 1.2);
    
    invertedViewTransform = viewTransform;
    if( invertedViewTransform.Invert() )
    {
      D2D1_POINT_2F inPoint { levelControlState.renderTargetMouseData.x, levelControlState.renderTargetMouseData.y };
      auto outPoint = invertedViewTransform.TransformPoint(inPoint);
      m_controlState->mouseX = mouseX = outPoint.x;
      m_controlState->mouseY = mouseY = outPoint.y;

      D2D1_POINT_2F screenTopLeft { 0, 0 };
      D2D1_POINT_2F screenBottomRight { levelControlState.renderTargetMouseData.size.width - 1, levelControlState.renderTargetMouseData.size.height - 1 };
      auto viewTopLeft = invertedViewTransform.TransformPoint(screenTopLeft);
      auto viewBottomRight = invertedViewTransform.TransformPoint(screenBottomRight);
      m_viewRect.topLeft = { viewTopLeft.x, viewTopLeft.y };
      m_viewRect.bottomRight = { viewBottomRight.x, viewBottomRight.y };
    }
  }
}

[[nodiscard]] auto level_state::CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, float renderScale) -> D2D1::Matrix3x2F
{
  return CreateGameLevelTransform(playerData->xPos, playerData->yPos, renderScale, renderTargetSize.width, renderTargetSize.height);
}

float level_state::GetUpdateInterval()
{
  int64_t intervalCount = currentTimerCount - previousTimerCount;
  return static_cast<float>(intervalCount) / static_cast<float>(counterFrequency) * gameSpeedMultiplier;
}

[[nodiscard]] auto level_state::PlayerIsDead() -> bool
{
  return playerData->state == player_ship::dead;
}

[[nodiscard]] auto level_state::TimedOut() -> bool
{
  return GetPlayTimeRemaining() > 0 ? false : true;
}

[[nodiscard]] auto level_state::GetPlayTimeRemaining() -> int64_t
{
  int64_t playTimeRemaining = levelTimeLimit - currentTimerCount;
  return max(0, playTimeRemaining);
}

[[nodiscard]] auto level_state::GetPlayTimeRemainingInSeconds() -> float
{
  return static_cast<float>(GetPlayTimeRemaining()) / static_cast<float>(counterFrequency);
}

auto level_state::RenderTo(ID2D1RenderTarget* renderTarget, const screen_render_data& renderData) -> void
{
  const auto renderBrushSelector = screen_render_brush_selector { renderData.renderBrushes };
  const auto textFormatSelector = screen_render_text_format_selector { renderData.textFormats };

  renderTarget->SetTransform(viewTransform);

  auto viewRect = GetViewRect(renderTarget);

  std::for_each(std::execution::seq, m_activeObjects.cbegin(), m_activeObjects.cend(), [renderTarget, viewRect](const auto& object)
  {
    object.RenderTo(renderTarget, viewRect);
  });
}

[[nodiscard]] auto level_state::GetViewRect(ID2D1RenderTarget* renderTarget) -> D2D1_RECT_F
{
  auto renderTargetSize = renderTarget->GetSize();
  auto renderTargetTopLeft  = D2D1_POINT_2F { 0, 0 };
  auto renderTargetBottomRight  = D2D1_POINT_2F { renderTargetSize.width - 1.0f, renderTargetSize.height - 1.0f };
  auto topLeft = invertedViewTransform.TransformPoint(renderTargetTopLeft);
  auto bottomRight = invertedViewTransform.TransformPoint(renderTargetBottomRight);
  return { topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };
}

auto level_state::PlaySoundEffects(const global_sound_buffer_selector& soundBuffers) -> void
{
  PlaySoundBuffer(soundBuffers[menu_theme], true);

  if( playerData->thrusterOn )
    PlaySoundBuffer(soundBuffers[thrust], true);
  else
    StopSoundBufferPlay(soundBuffers[thrust]);

  if( playerShot )
  {
    ResetSoundBuffer(soundBuffers[shoot]);
    PlaySoundBuffer(soundBuffers[shoot]);
  }

  if( targetShot )
  {
    ResetSoundBuffer(soundBuffers[shoot]);
    PlaySoundBuffer(soundBuffers[target_activated]);
  }
}
