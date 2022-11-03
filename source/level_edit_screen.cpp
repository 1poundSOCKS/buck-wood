#include "level_edit_screen.h"
#include <numeric>
#include "render.h"
#include "game_math.h"

std::pair<std::list<game_point>::iterator, float> GetClosestPointWithin(const level_edit_screen_state& screenState, float x, float y, float distanceWithin);
std::pair<std::list<game_point>::iterator, float> GetClosestPointWithin(std::list<game_point>::iterator first, std::list<game_point>::iterator last, float x, float y, float distanceWithin);

level_edit_screen_state::level_edit_screen_state(const global_state& globalState)
: globalState(globalState),
  brushes(globalState.brushes),
  textFormats(globalState.textFormats),
  gameLevelDataIndex(*globalState.gameLevelDataIndex)
{
  currentLevelDataIterator = gameLevelDataIndex.begin();
  const auto& levelData = *currentLevelDataIterator;
  currentLevel = std::make_unique<game_level_edit>(*levelData);

  playerShip.xPos = currentLevel->playerStartPosX;
  playerShip.yPos = currentLevel->playerStartPosY;
}

D2D1::Matrix3x2F CreateViewTransform(const winrt::com_ptr<ID2D1RenderTarget>& renderTarget, const level_edit_screen_state& screenState)
{
  static const float scale = 0.8f;

  auto renderTargetSize = renderTarget->GetSize();
  return CreateGameLevelTransform(screenState.levelCenterX, screenState.levelCenterY, scale, renderTargetSize.width, renderTargetSize.height);
}

void RefreshControlState(level_edit_control_state& controlState, const d2d_app& app, const D2D1::Matrix3x2F& worldViewTransform)
{
  RefreshControlState(controlState.controlState, app, worldViewTransform);

  // bool leftMouseDrag = false, rightMouseDrag = false;

  // controlState.previousControlState = controlState.controlState;
}

void RenderFrame(const d2d_frame& frame, const level_edit_screen_state& screenState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  RenderLevel(frame, *screenState.currentLevel, screenState.brushes);

  RenderPlayer(frame, screenState.playerShip, screenState.brushes);

  if( screenState.closestPoint )
    RenderHighlightedPoint(frame, *screenState.closestPoint, screenState.brushes);

  RenderMouseCursor(frame, screenState.mouseCursor, screenState.brushes);
}

void UpdateScreenState(level_edit_screen_state& screenState, const level_edit_control_state& controlState, const system_timer& timer)
{
  const control_state& baseControlState = controlState.controlState;

  screenState.levelMouseX = baseControlState.worldMouseX;
  screenState.levelMouseY = baseControlState.worldMouseY;

  screenState.returnToMenu = false;

  if( baseControlState.quitPress ) screenState.returnToMenu = true;

  if( baseControlState.mouseX < 0.1f ) screenState.levelCenterX -= 10.0f;
  else if( baseControlState.mouseX > 0.9f ) screenState.levelCenterX += 10.0f;

  if( baseControlState.mouseY < 0.1f ) screenState.levelCenterY -= 10.0f;
  else if( baseControlState.mouseY > 0.9f ) screenState.levelCenterY += 10.0f;

  screenState.closestPoint = nullptr;

  const auto& closestPoint = GetClosestPointWithin(screenState, baseControlState.worldMouseX, baseControlState.worldMouseY, 100.0f);

  if( closestPoint.second < 100.0f )
  {
    auto& point = *closestPoint.first;
    screenState.closestPoint = &point;
    if( baseControlState.shoot )
    {
      screenState.closestPoint->x = baseControlState.worldMouseX;
      screenState.closestPoint->y = baseControlState.worldMouseY;
    }
    else if( baseControlState.accelerate )
    {
      screenState.currentLevel->boundary->points.insert(closestPoint.first, game_point(baseControlState.worldMouseX, baseControlState.worldMouseY));
    }
  }
}

std::pair<std::list<game_point>::iterator, float> GetClosestPointWithin(const level_edit_screen_state& screenState, float x, float y, float distanceWithin)
{
  const auto& level = *screenState.currentLevel;

  auto closestPoint = GetClosestPointWithin(
    level.boundary->points.begin(),
    level.boundary->points.end(),
    x, y, distanceWithin);

  for( const auto& object : level.objects )
  {
    auto closestObjectPoint = GetClosestPointWithin(
      object->points.begin(),
      object->points.end(),
      x, y, distanceWithin);

    if( closestObjectPoint.second < closestPoint.second ) closestPoint = closestObjectPoint;
  }

  return closestPoint;
}

std::pair<std::list<game_point>::iterator, float> GetClosestPointWithin(std::list<game_point>::iterator first, std::list<game_point>::iterator last, float x, float y, float distanceWithin)
{
  float closestPointDistance = distanceWithin;

  std::list<game_point>::iterator closestPoint = last;

  for( std::list<game_point>::iterator i = first; i != last; i++ )
  {
    float distance = GetDistanceBetweenPoints(i->x, i->y, x, y);
    if( distance < closestPointDistance )
    {
      closestPointDistance = distance;
      closestPoint = i;
    }
  }

  return std::pair<std::list<game_point>::iterator, float>(closestPoint, closestPointDistance);
}

void FormatDiagnostics(diagnostics_data& diagnosticsData, const level_edit_screen_state& screenState, const level_edit_control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
  const control_state& baseControlState = controlState.controlState;

  FormatDiagnostics(diagnosticsData, baseControlState, perfData, timer);

  static wchar_t text[64];
  swprintf(text, L"level mouse X: %.1f", screenState.levelMouseX);
  diagnosticsData.push_back(text);

  swprintf(text, L"level mouse Y: %.1f", screenState.levelMouseY);
  diagnosticsData.push_back(text);

  if( screenState.closestPoint )
  {
    swprintf(text, L"nearest point: %.1f, %.1f", screenState.closestPoint->x, screenState.closestPoint->y);
    diagnosticsData.push_back(text);
  }
}

void UpdateGlobalState(global_state& globalState, const level_edit_screen_state& screenState)
{
  //  auto& firstLevelData = globalState.gameLevelDataIndex->front();
  // UpdateGameLevelData(*firstLevelData, *screenState.currentLevel);
}
