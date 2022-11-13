#include "level_edit_screen.h"
#include <numeric>
#include "render.h"
#include "game_math.h"

D2D1::Matrix3x2F CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, const level_edit_screen_state& screenState);
void UpdateScreenExitState(level_edit_screen_state& screenState, const level_edit_control_state& controlState);
void UpdateScreenStateMouseData(level_edit_screen_state& screenState, const level_edit_control_state& controlState);
void RunDragDrop(level_edit_screen_state& screenState, const level_edit_control_state& controlState);
std::unique_ptr<game_level_data> CreateGameLevelData(const drag_drop_state& dragDropState);
void CreateDragDropPoints(std::vector<game_point>::const_iterator begin, std::vector<game_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator);
void CreateGamePoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<game_point>> gamePointInserter);

level_edit_screen_state::level_edit_screen_state(const global_state& globalState)
: globalState(globalState),
  brushes(globalState.brushes),
  textFormats(globalState.textFormats),
  gameLevelDataIndex(*globalState.gameLevelDataIndex)
{
  currentLevelDataIterator = gameLevelDataIndex.gameLevelData.begin();
  const auto& levelData = *currentLevelDataIterator;

  dragDropState.shapes.resize(1 + levelData->objects.size());
  int shapeIndex = 0;
  CreateDragDropPoints(levelData->boundaryPoints.cbegin(), levelData->boundaryPoints.cend(), std::back_inserter(dragDropState.shapes[shapeIndex++].points));
  
  for( const auto& object : levelData->objects )
  {
    CreateDragDropPoints(object->points.cbegin(), object->points.cend(), std::back_inserter(dragDropState.shapes[shapeIndex++].points));
  }

  dragDropState.objects.resize(1 + levelData->targets.size());
  int objectIndex = 0;
  dragDropState.objects[objectIndex].x = levelData->playerStartPosX;
  dragDropState.objects[objectIndex++].y = levelData->playerStartPosY;
  for( const auto& target: levelData->targets )
  {
    dragDropState.objects[objectIndex].x = target.x;
    dragDropState.objects[objectIndex++].y = target.y;
  }
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
  controlState.returnToMenu = controlState.cancelExit = baseControlState.escapeKeyPress;
  controlState.saveChanges = baseControlState.keyPress_y;
  controlState.discardChanges = baseControlState.keyPress_n;
  controlState.leftMouseButtonDown = baseControlState.leftMouseButtonDown;
  controlState.rightMouseButtonDown = baseControlState.rightMouseButtonDown;
  controlState.renderTargetMouseData = baseControlState.renderTargetMouseData;
  controlState.ratioMouseX = controlState.renderTargetMouseData.x / controlState.renderTargetMouseData.size.width;
  controlState.ratioMouseY = controlState.renderTargetMouseData.y / controlState.renderTargetMouseData.size.height;
  controlState.leftMouseButtonReleased = baseControlState.leftMouseButtonReleased;
  controlState.rightMouseButtonReleased = baseControlState.rightMouseButtonReleased;
  controlState.rightMouseButtonDrag = baseControlState.rightMouseButtonDrag;
  controlState.leftMouseButtonDrag = baseControlState.leftMouseButtonDrag;
  controlState.rightMouseButtonDrag = baseControlState.rightMouseButtonDrag;
  controlState.deleteItem = baseControlState.deleteKeyPress;
}

void RenderFrame(const d2d_frame& frame, const level_edit_screen_state& screenState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  if( screenState.viewState == level_edit_screen_state::view_exit )
  {
    RenderMainScreenPrompt(frame.renderTarget, screenState.textFormats.menuTextFormat, screenState.brushes.brushLevelEndText, L"save changes (y/n)");
    return;
  }

  frame.renderTarget->SetTransform(screenState.viewTransform);

  std::vector<render_line> renderLines;
  CreateRenderLines(renderLines, screenState.dragDropState);
  RenderLines(frame.renderTarget, screenState.globalState.renderBrushes, 2, renderLines.begin(), renderLines.end());

  std::vector<render_point> renderPoints;
  CreateRenderPoints(renderPoints, screenState.dragDropState);
  RenderPoints(frame.renderTarget, screenState.globalState.renderBrushes, renderPoints.cbegin(), renderPoints.cend());

  RenderMouseCursor(frame.renderTarget, screenState.mouseCursor, screenState.mouseX, screenState.mouseY, screenState.brushes);
}

void UpdateScreenState(level_edit_screen_state& screenState, const level_edit_control_state& controlState, const system_timer& timer)
{
  if( screenState.viewState == level_edit_screen_state::view_exit )
  {
    UpdateScreenExitState(screenState, controlState);
    return;
  }

  if( controlState.returnToMenu )
  {
    screenState.viewState = level_edit_screen_state::view_exit;
    return;
  }

  UpdateScreenStateMouseData(screenState, controlState);

  if( controlState.ratioMouseX < 0.1f ) screenState.levelCenterX -= 10.0f;
  else if( controlState.ratioMouseX > 0.9f ) screenState.levelCenterX += 10.0f;

  if( controlState.ratioMouseY < 0.1f ) screenState.levelCenterY -= 10.0f;
  else if( controlState.ratioMouseY > 0.9f ) screenState.levelCenterY += 10.0f;

  RunDragDrop(screenState, controlState);
}

void UpdateScreenExitState(level_edit_screen_state& screenState, const level_edit_control_state& controlState)
{
  if( controlState.cancelExit )
  {
    screenState.viewState = level_edit_screen_state::view_default;
  }
  else if( controlState.discardChanges )
  {
    screenState.saveChanges = false;
    screenState.returnToMenu = true;
  }
  else if( controlState.saveChanges )
  {
    screenState.saveChanges = true;
    screenState.returnToMenu = true;
  }
}

void UpdateScreenStateMouseData(level_edit_screen_state& screenState, const level_edit_control_state& controlState)
{
  screenState.mouseX = controlState.renderTargetMouseData.x;
  screenState.mouseY = controlState.renderTargetMouseData.y;

  screenState.viewTransform = CreateViewTransform(controlState.renderTargetMouseData.size, screenState);

  auto mouseTransform = screenState.viewTransform;

  if( mouseTransform.Invert() )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = controlState.renderTargetMouseData.x;
    inPoint.y = controlState.renderTargetMouseData.y;
    auto outPoint = mouseTransform.TransformPoint(inPoint);
    screenState.levelMouseX = outPoint.x;
    screenState.levelMouseY = outPoint.y;
  }
  else
  {
    screenState.levelMouseX = 0;
    screenState.levelMouseY = 0;
  }
}

void RunDragDrop(level_edit_screen_state& screenState, const level_edit_control_state& controlState)
{
  drag_drop_control_state dragDropControlState;
  dragDropControlState.leftMouseButtonDown = controlState.leftMouseButtonDown;
  dragDropControlState.leftMouseButtonDrag = controlState.leftMouseButtonDrag;
  dragDropControlState.leftMouseButtonReleased = controlState.leftMouseButtonReleased;
  dragDropControlState.mouseX = screenState.levelMouseX;
  dragDropControlState.mouseY = screenState.levelMouseY;
  dragDropControlState.deleteItem = controlState.deleteItem;

  ProcessDragDrop(screenState.dragDropState, dragDropControlState);
}

D2D1::Matrix3x2F CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, const level_edit_screen_state& screenState)
{
  static const float scale = 0.8f;
  return CreateGameLevelTransform(screenState.levelCenterX, screenState.levelCenterY, scale, renderTargetSize.width, renderTargetSize.height);
}

void FormatDiagnostics(diagnostics_data& diagnosticsData, const level_edit_screen_state& screenState, const level_edit_control_state& controlState)
{
  static wchar_t text[64];
  swprintf(text, L"level mouse X: %.1f", screenState.levelMouseX);
  diagnosticsData.push_back(text);

  swprintf(text, L"level mouse Y: %.1f", screenState.levelMouseY);
  diagnosticsData.push_back(text);

  FormatDiagnostics(diagnosticsData, screenState.dragDropState);
}

void UpdateGlobalState(global_state& globalState, const level_edit_screen_state& screenState)
{
  auto& levelDataIterator = globalState.gameLevelDataIndex->gameLevelData.begin();
  *levelDataIterator = std::move(CreateGameLevelData(screenState.dragDropState));

  globalState.gameLevelDataIndexUpdated = true;
}

std::unique_ptr<game_level_data> CreateGameLevelData(const drag_drop_state& dragDropState)
{
  auto gameLevelData = std::make_unique<game_level_data>();

  const auto& dragDropShape = *dragDropState.shapes.begin();
  CreateGamePoints(dragDropShape.points.cbegin(), dragDropShape.points.cend(), std::back_inserter(gameLevelData->boundaryPoints));

  return gameLevelData;
}

void CreateDragDropPoints(std::vector<game_point>::const_iterator begin, std::vector<game_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator)
{
  std::transform(begin, end, insertIterator, [](const game_point& point)
  {
    return drag_drop_point(point.x, point.y, drag_drop_point::type::type_real);
  });
}

void CreateGamePoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<game_point>> gamePointInserter)
{
  std::list<drag_drop_point> realPoints;
  std::copy_if(begin, end, std::back_inserter(realPoints), [](const auto& point)
  {
    return point.pointType == drag_drop_point::type::type_real;
  });

  std::transform(realPoints.cbegin(), realPoints.cend(), gamePointInserter, [](const auto& dragDropPoint)
  {
    return game_point(dragDropPoint.x, dragDropPoint.y);
  });
}
