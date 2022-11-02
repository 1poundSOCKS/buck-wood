#include "level_edit_screen.h"
#include <numeric>
#include "render.h"
#include "game_math.h"

int GetClosestPointIndex(float x, float y, std::vector<game_point> points);
void FetchAllLevelPoints(const game_level_edit& level, std::vector<std::reference_wrapper<game_point>>& points);
game_point& GetClosestPoint(float x, float y, std::vector<std::reference_wrapper<game_point>> points, game_point& default);

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

void RenderFrame(const d2d_frame& frame, const level_edit_screen_state& screenState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  RenderLevel(frame, *screenState.currentLevel, screenState.brushes);

  RenderPlayer(frame, screenState.playerShip, screenState.brushes);

  if( screenState.closestPointRef )
  {
    const game_point& point = screenState.closestPointRef->get();
    RenderHighlightedPoint(frame, point, screenState.brushes);
  }

  RenderMouseCursor(frame, screenState.mouseCursor, screenState.brushes);
}

void UpdateScreenState(level_edit_screen_state& screenState, const control_state& controlState, const system_timer& timer)
{
  screenState.levelMouseX = controlState.worldMouseX;
  screenState.levelMouseY = controlState.worldMouseY;

  screenState.returnToMenu = false;

  if( controlState.quitPress ) screenState.returnToMenu = true;

  if( controlState.mouseX < 0.1f ) screenState.levelCenterX -= 10.0f;
  else if( controlState.mouseX > 0.9f ) screenState.levelCenterX += 10.0f;

  if( controlState.mouseY < 0.1f ) screenState.levelCenterY -= 10.0f;
  else if( controlState.mouseY > 0.9f ) screenState.levelCenterY += 10.0f;

  screenState.closestPointRef = nullptr;

  std::vector<std::reference_wrapper<game_point>> allPoints;
  FetchAllLevelPoints(*screenState.currentLevel, allPoints);
  if( allPoints.size() )
  {
    game_point& default = allPoints[0].get();
    game_point& closestPoint = GetClosestPoint(screenState.levelMouseX, screenState.levelMouseY, allPoints, default);
    screenState.closestPointRef = std::make_unique<std::reference_wrapper<game_point>>(std::ref(closestPoint));
  }

  if( controlState.shoot && screenState.closestPointRef != nullptr )
  {
    game_point& point = screenState.closestPointRef->get();
    point.x = controlState.worldMouseX;
    point.y = controlState.worldMouseY;
  }
}

game_point& GetClosestPoint(float x, float y, std::vector<std::reference_wrapper<game_point>> points, game_point& default)
{
  float closestPointDistance = 100.0f;
  int closestPointIndex = -1;

  for( int i=0; i < points.size(); i++ )
  {
    float distance = GetDistanceBetweenPoints(x, y, points[i].get().x, points[i].get().y);
    if( distance < closestPointDistance )
    {
      closestPointDistance = distance;
      closestPointIndex = i;
    }
  }

  return ( closestPointIndex == - 1 ) ? default : points[closestPointIndex].get();
}

void FetchAllLevelPoints(const game_level_edit& level, std::vector<std::reference_wrapper<game_point>>& points)
{
  std::vector<size_t> objectSizes;
  objectSizes.reserve(level.objects.size());
  std::transform(level.objects.cbegin(), level.objects.cend(), std::back_inserter(objectSizes), [](const std::unique_ptr<game_shape_edit>& shape) { return shape->points.size(); });
  size_t sumOfObjectSizes = std::accumulate(objectSizes.begin(), objectSizes.end(), static_cast<size_t>(0));

  points.clear();
  points.reserve(level.boundary->points.size() + sumOfObjectSizes);

  std::transform(level.boundary->points.begin(), level.boundary->points.end(), std::back_inserter(points), [](game_point& point) { return std::ref(point); });

  for( const auto& object : level.objects )
  {
    std::transform(object->points.begin(), object->points.end(), std::back_inserter(points), [](game_point& point) { return std::ref(point); } );
  }
}

void FormatDiagnostics(diagnostics_data& diagnosticsData, const level_edit_screen_state& screenState, const control_state& controlState, const perf_data& perfData, const system_timer& timer)
{
  FormatDiagnostics(diagnosticsData, controlState, perfData, timer);

  static wchar_t text[64];
  swprintf(text, L"level mouse X: %.1f", screenState.levelMouseX);
  diagnosticsData.push_back(text);

  swprintf(text, L"level mouse Y: %.1f", screenState.levelMouseY);
  diagnosticsData.push_back(text);

  if( screenState.closestPointRef )
  {
    swprintf(text, L"nearest point: %.1f, %.1f", screenState.closestPointRef->get().x, screenState.closestPointRef->get().y);
    diagnosticsData.push_back(text);
  }
}

void UpdateGlobalState(global_state& globalState, const level_edit_screen_state& screenState)
{
  //  auto& firstLevelData = globalState.gameLevelDataIndex->front();
  // UpdateGameLevelData(*firstLevelData, *screenState.currentLevel);
}
