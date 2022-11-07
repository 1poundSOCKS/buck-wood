#include "level_edit_screen.h"
#include <numeric>
#include "render.h"
#include "game_math.h"

D2D1::Matrix3x2F CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, const level_edit_screen_state& screenState);
void RunDragDropForBorderAndObjects(level_edit_screen_state& screenState, const level_edit_control_state& controlState);
void RunDragDropForTargets(level_edit_screen_state& screenState, const level_edit_control_state& controlState);
void RunDragDropForPlayer(level_edit_screen_state& screenState, const level_edit_control_state& controlState);
void GetHighlightedTarget(level_edit_screen_state& screenState, const level_edit_control_state& controlState);
std::unique_ptr<game_point_selection> GetClosestPoint(const level_edit_screen_state& screenState, float x, float y);
std::unique_ptr<game_point_selection> GetClosestPoint(std::list<game_point>& points, float x, float y);

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

game_point_selection::game_point_selection(std::list<game_point>& points, std::list<game_point>::iterator& point, float distance)
: points(points), point(point), distance(distance)
{
}

target_selection::target_selection(std::list<target_edit>& targets, std::list<target_edit>::iterator& target)
: targets(targets), target(target)
{
}

void RefreshControlState(level_edit_control_state& controlState, const control_state& baseControlState)
{
  controlState.returnToMenu = baseControlState.escapeKeyPress;

  controlState.leftMouseButtonDown = baseControlState.leftMouseButtonDown;
  controlState.rightMouseButtonDown = baseControlState.rightMouseButtonDown;

  controlState.ratioMouseX = baseControlState.ratioMouseX;
  controlState.ratioMouseY = baseControlState.ratioMouseY;

  controlState.renderTargetMouseX = baseControlState.renderTargetMouseX;
  controlState.renderTargetMouseY = baseControlState.renderTargetMouseY;

  controlState.leftMouseButtonDrag = baseControlState.leftMouseButtonDrag;
  controlState.rightMouseButtonDrag = baseControlState.rightMouseButtonDrag;
}

void RenderFrame(const d2d_frame& frame, const level_edit_screen_state& screenState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  RenderLevel(frame.renderTarget, screenState.viewTransform, *screenState.currentLevel, screenState.brushes);

  const winrt::com_ptr<ID2D1SolidColorBrush>& playerBrush = 
    screenState.playerHighlighted || screenState.playerDrag ? screenState.brushes.brushActivated : screenState.brushes.brush;

  RenderPlayer(frame.renderTarget, screenState.viewTransform, screenState.playerShip, playerBrush, screenState.brushes.brushThrusters);

  if( screenState.closestPoint )
    RenderHighlightedPoint(frame.renderTarget, screenState.viewTransform, *screenState.closestPoint->point, screenState.brushes);

  if( screenState.dragPoint )
    RenderHighlightedPoint(frame.renderTarget, screenState.viewTransform, *screenState.dragPoint->point, screenState.brushes);

  if( screenState.highlightedTarget )
    RenderTarget(frame.renderTarget, screenState.viewTransform, *screenState.highlightedTarget->target, screenState.brushes.brushActivated);

  if( screenState.dragTarget )
    RenderTarget(frame.renderTarget, screenState.viewTransform, *screenState.dragTarget->target, screenState.brushes.brushActivated);

  RenderMouseCursor(frame.renderTarget, screenState.mouseCursor, screenState.mouseX, screenState.mouseY, screenState.brushes);
}

void UpdateScreenState(level_edit_screen_state& screenState, const D2D1_SIZE_F& renderTargetSize, const level_edit_control_state& controlState, const system_timer& timer)
{
  screenState.mouseX = controlState.renderTargetMouseX;
  screenState.mouseY = controlState.renderTargetMouseY;

  screenState.viewTransform = CreateViewTransform(renderTargetSize, screenState);

  auto mouseTransform = screenState.viewTransform;

  if( mouseTransform.Invert() )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = controlState.renderTargetMouseX;
    inPoint.y = controlState.renderTargetMouseY;
    auto outPoint = mouseTransform.TransformPoint(inPoint);
    screenState.levelMouseX = outPoint.x;
    screenState.levelMouseY = outPoint.y;
  }
  else
  {
    screenState.levelMouseX = 0;
    screenState.levelMouseY = 0;
  }

  screenState.returnToMenu = false;

  if( controlState.returnToMenu ) screenState.returnToMenu = true;

  if( controlState.ratioMouseX < 0.1f ) screenState.levelCenterX -= 10.0f;
  else if( controlState.ratioMouseX > 0.9f ) screenState.levelCenterX += 10.0f;

  if( controlState.ratioMouseY < 0.1f ) screenState.levelCenterY -= 10.0f;
  else if( controlState.ratioMouseY > 0.9f ) screenState.levelCenterY += 10.0f;

  RunDragDropForBorderAndObjects(screenState, controlState);

  RunDragDropForTargets(screenState, controlState);

  RunDragDropForPlayer(screenState, controlState);
}

void RunDragDropForBorderAndObjects(level_edit_screen_state& screenState, const level_edit_control_state& controlState)
{
  if( screenState.closestPoint )
  {
    if( controlState.leftMouseButtonDown )
    {
      screenState.dragPoint = std::move(screenState.closestPoint);
    }
    else if( controlState.rightMouseButtonDown)
    {
      game_point newPoint = *screenState.closestPoint->point;
      std::list<game_point>::iterator i = screenState.closestPoint->points.insert(screenState.closestPoint->point, newPoint);
      screenState.dragPoint = std::make_unique<game_point_selection>(screenState.closestPoint->points, i, screenState.closestPoint->distance);
    }
  }

  screenState.closestPoint = nullptr;

  if( !controlState.leftMouseButtonDown && !controlState.rightMouseButtonDown )
  {
    screenState.closestPoint = GetClosestPoint(screenState, screenState.levelMouseX, screenState.levelMouseY);
    if( screenState.closestPoint->distance > 50 ) screenState.closestPoint = nullptr;
    screenState.dragPoint = nullptr;
  }

  else if( screenState.dragPoint && controlState.leftMouseButtonDrag )
  {
    screenState.dragPoint->point->x = screenState.levelMouseX;
    screenState.dragPoint->point->y = screenState.levelMouseY;
  }

  else if( screenState.dragPoint && controlState.rightMouseButtonDrag )
  {
    screenState.dragPoint->point->x = screenState.levelMouseX;
    screenState.dragPoint->point->y = screenState.levelMouseY;
  }
}

void RunDragDropForTargets(level_edit_screen_state& screenState, const level_edit_control_state& controlState)
{
  if( !controlState.leftMouseButtonDown )
  {
    GetHighlightedTarget(screenState, controlState);
    screenState.dragTarget = nullptr;
  }

  if( screenState.highlightedTarget && controlState.leftMouseButtonDown )
  {
    screenState.dragTarget = std::move(screenState.highlightedTarget);
  }

  if( screenState.dragTarget && controlState.leftMouseButtonDrag )
  {
    auto& target = *screenState.dragTarget->target;
    target.x = screenState.levelMouseX;
    target.y = screenState.levelMouseY;
  }
}

void RunDragDropForPlayer(level_edit_screen_state& screenState, const level_edit_control_state& controlState)
{
  if( !controlState.leftMouseButtonDown )
  {
    game_shape playerShape;
    TransformPlayerShip(screenState.playerShip, playerShape);
    screenState.playerHighlighted = PointInside(game_point(screenState.levelMouseX, screenState.levelMouseY), playerShape);
    screenState.playerDrag = false;
  }

  if( screenState.playerHighlighted && controlState.leftMouseButtonDown )
  {
    screenState.playerDrag = true;
    screenState.playerHighlighted = false;
  }

  if( screenState.playerDrag && controlState.leftMouseButtonDrag )
  {
    screenState.playerShip.xPos = screenState.currentLevel->playerStartPosX = screenState.levelMouseX;
    screenState.playerShip.yPos = screenState.currentLevel->playerStartPosY = screenState.levelMouseY;
  }
}

void GetHighlightedTarget(level_edit_screen_state& screenState, const level_edit_control_state& controlState)
{
  screenState.highlightedTarget = nullptr;

  auto& targets = screenState.currentLevel->targets;

  for( auto i = targets.begin(); i != targets.end(); i++ )
  {
    auto& target = *i;

    game_shape targetShape;
    InitializeTargetShape(target.x, target.y, target.size, targetShape);

    if( PointInside(game_point(screenState.levelMouseX, screenState.levelMouseY), targetShape) )
    {
      screenState.highlightedTarget = std::make_unique<target_selection>(targets, i);
    }
  }
}

D2D1::Matrix3x2F CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, const level_edit_screen_state& screenState)
{
  static const float scale = 0.8f;
  return CreateGameLevelTransform(screenState.levelCenterX, screenState.levelCenterY, scale, renderTargetSize.width, renderTargetSize.height);
}

std::unique_ptr<game_point_selection> GetClosestPoint(const level_edit_screen_state& screenState, float x, float y)
{
  const auto& level = *screenState.currentLevel;

  auto closestPoint = GetClosestPoint(level.boundary->points, x, y);

  for( const auto& object : level.objects )
  {
    auto closestObjectPoint = GetClosestPoint(object->points, x, y);
    if( closestObjectPoint->distance < closestPoint->distance ) closestPoint = std::move(closestObjectPoint);
  }

  return closestPoint;
}

std::unique_ptr<game_point_selection> GetClosestPoint(std::list<game_point>& points, float x, float y)
{
  std::unique_ptr<game_point_selection> closestPoint = nullptr;

  for( auto i = points.begin(); i != points.end(); i++ )
  {
    float distance = GetDistanceBetweenPoints(i->x, i->y, x, y);
    if( closestPoint == nullptr || distance < closestPoint->distance )
      closestPoint = std::make_unique<game_point_selection>(points, i, distance);
  }

  return closestPoint;
}

void FormatDiagnostics(diagnostics_data& diagnosticsData, const level_edit_screen_state& screenState, const level_edit_control_state& controlState)
{
  static wchar_t text[64];
  swprintf(text, L"level mouse X: %.1f", screenState.levelMouseX);
  diagnosticsData.push_back(text);

  swprintf(text, L"level mouse Y: %.1f", screenState.levelMouseY);
  diagnosticsData.push_back(text);

  if( screenState.closestPoint )
  {
    swprintf(text, L"nearest point: %.1f, %.1f", screenState.closestPoint->point->x, screenState.closestPoint->point->y);
    diagnosticsData.push_back(text);
  }
}

void UpdateGlobalState(global_state& globalState, const level_edit_screen_state& screenState)
{
  auto& firstLevelData = globalState.gameLevelDataIndex->front();
  UpdateGameLevelData(*firstLevelData, *screenState.currentLevel);
}
