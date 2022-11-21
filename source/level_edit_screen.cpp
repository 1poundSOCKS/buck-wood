#include "pch.h"
#include "level_edit_screen.h"
#include "render.h"
#include "game_math.h"

D2D1::Matrix3x2F CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, const level_edit_screen_state& screenState);
void UpdateScreenExitState(level_edit_screen_state& screenState, const level_edit_control_state& controlState);
void UpdateScreenStateMouseData(level_edit_screen_state& screenState, const level_edit_control_state& controlState);
void RunDragDrop(level_edit_screen_state& screenState, const level_edit_control_state& controlState);
void LoadCurrentLevel(level_edit_screen_state& screenState);
void SaveCurrentLevel(level_edit_screen_state& screenState);
std::unique_ptr<drag_drop_state> CreateDragDropState(const game_level_data& gameLevelData);
std::unique_ptr<game_level_data> CreateGameLevelData(const drag_drop_state& dragDropState);
std::unique_ptr<game_level_object_data> CreateGameLevelObjectData(const drag_drop_shape& dragDropShape);
void CreateDragDropPoints(std::vector<game_point>::const_iterator begin, std::vector<game_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator);
void CreateDragDropPoints(std::list<game_point>::const_iterator begin, std::list<game_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator);
void CreateGamePoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<game_point>> gamePointInserter);

level_edit_screen_state::level_edit_screen_state(const global_state& globalState)
: globalState(globalState),
  renderBrushes(globalState.renderBrushes),
  textFormats(globalState.textFormats),
  gameLevelDataIndex(*globalState.gameLevelDataIndex)
{
  currentLevelDataIterator = gameLevelDataIndex.gameLevelData.begin();
  LoadCurrentLevel(*this);
}

void RefreshControlState(level_edit_control_state& controlState, const control_state& baseControlState)
{
  controlState.returnToMenu = controlState.cancelExit = baseControlState.escapeKeyPress;
  controlState.saveChanges = baseControlState.keyPress_y;
  controlState.discardChanges = baseControlState.keyPress_n;
  controlState.renderTargetMouseData = baseControlState.renderTargetMouseData;
  controlState.ratioMouseX = controlState.renderTargetMouseData.x / controlState.renderTargetMouseData.size.width;
  controlState.ratioMouseY = controlState.renderTargetMouseData.y / controlState.renderTargetMouseData.size.height;
  controlState.nextLevel = baseControlState.f1Press;
  controlState.previousLevel = baseControlState.f2Press;
  controlState.dragDropControlState.leftMouseButtonDown = baseControlState.leftMouseButtonDown;
  controlState.dragDropControlState.leftMouseButtonDrag = baseControlState.leftMouseButtonDrag;
  controlState.dragDropControlState.leftMouseButtonReleased = baseControlState.leftMouseButtonReleased;
  controlState.dragDropControlState.deleteItem = baseControlState.deleteKeyPress;
  controlState.dragDropControlState.mouseX = 0;
  controlState.dragDropControlState.mouseY = 0;
}

void RenderFrame(const d2d_frame& frame, const level_edit_screen_state& screenState)
{
  frame.renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  if( screenState.viewState == level_edit_screen_state::view_exit )
  {
    RenderMainScreenPrompt(frame.renderTarget, screenState.textFormats.menuTextFormat, screenState.renderBrushes.brushCyan, L"save changes (y/n)");
    return;
  }

  frame.renderTarget->SetTransform(screenState.viewTransform);

  std::vector<render_line> renderLines;
  CreateRenderLines(renderLines, *screenState.dragDropState);
  RenderLines(frame.renderTarget, screenState.globalState.renderBrushes, 2, renderLines.begin(), renderLines.end());

  std::vector<render_point> renderPoints;
  CreateRenderPoints(renderPoints, *screenState.dragDropState);
  RenderPoints(frame.renderTarget, screenState.globalState.renderBrushes, renderPoints.cbegin(), renderPoints.cend());
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
    SaveCurrentLevel(screenState);
    screenState.viewState = level_edit_screen_state::view_exit;
    return;
  }

  UpdateScreenStateMouseData(screenState, controlState);

  if( controlState.nextLevel )
  {
    auto nextLevelData = std::next(screenState.currentLevelDataIterator);
    
    if( nextLevelData != screenState.gameLevelDataIndex.gameLevelData.end() )
    {
      SaveCurrentLevel(screenState);
      screenState.currentLevelDataIterator = nextLevelData;
      LoadCurrentLevel(screenState);
    }
  }

  if( controlState.previousLevel )
  {
    if( screenState.currentLevelDataIterator != screenState.gameLevelDataIndex.gameLevelData.begin() )
    {
      SaveCurrentLevel(screenState);
      screenState.currentLevelDataIterator--;
      LoadCurrentLevel(screenState);
    }
  }

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
  drag_drop_control_state dragDropControlState = controlState.dragDropControlState;
  dragDropControlState.mouseX = screenState.levelMouseX;
  dragDropControlState.mouseY = screenState.levelMouseY;

  ProcessDragDrop(*screenState.dragDropState, dragDropControlState);
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

  FormatDiagnostics(diagnosticsData, *screenState.dragDropState);
}

void UpdateGlobalState(global_state& globalState, const level_edit_screen_state& screenState)
{
  globalState.gameLevelDataIndex = std::make_unique<game_level_data_index>(screenState.gameLevelDataIndex);
  globalState.gameLevelDataIndexUpdated = true;
}

void LoadCurrentLevel(level_edit_screen_state& screenState)
{
  const auto& levelData = *screenState.currentLevelDataIterator;

  if( levelData )
  {
    screenState.levelName = levelData->name;
    screenState.levelFilename = levelData->filename;
    screenState.levelTimeLimit = levelData->timeLimitInSeconds;
    screenState.dragDropState = CreateDragDropState(*levelData);
  }
}

void SaveCurrentLevel(level_edit_screen_state& screenState)
{
  auto gameLevelData = CreateGameLevelData(*screenState.dragDropState);
  gameLevelData->name = screenState.levelName;
  gameLevelData->filename = screenState.levelFilename;
  gameLevelData->timeLimitInSeconds = screenState.levelTimeLimit;
  *screenState.currentLevelDataIterator = std::move(gameLevelData);
}

std::unique_ptr<drag_drop_state> CreateDragDropState(const game_level_data& gameLevelData)
{
  auto dragDropState = std::make_unique<drag_drop_state>();
  
  drag_drop_shape boundaryShape(level_edit_screen_state::drag_drop_shape_type::type_boundary);
  CreateDragDropPoints(gameLevelData.boundaryPoints.cbegin(), gameLevelData.boundaryPoints.cend(), std::back_inserter(boundaryShape.points));
  dragDropState->shapes.push_back(boundaryShape);
  
  for( const auto& object : gameLevelData.objects )
  {
    drag_drop_shape objectShape(level_edit_screen_state::drag_drop_shape_type::type_object);
    CreateDragDropPoints(object.points.cbegin(), object.points.cend(), std::back_inserter(objectShape.points));
    dragDropState->shapes.push_back(objectShape);
  }

  drag_drop_shape playerShape(level_edit_screen_state::drag_drop_shape_type::type_player);
  playerShape.fixedShape = true;
  playerShape.position.x = gameLevelData.playerStartPosX;
  playerShape.position.y = gameLevelData.playerStartPosY;
  std::vector<game_point> points;
  CreatePointsForPlayer(std::back_inserter(points));
  CreateDragDropPoints(points.cbegin(), points.cend(), std::back_inserter(playerShape.points));
  dragDropState->shapes.push_back(playerShape);

  for( const auto& targetPos : gameLevelData.targets )
  {
    std::vector<game_point> targetPoints;
    CreatePointsForTarget(defaultTargetSize, std::back_inserter(targetPoints));
    drag_drop_shape targetShape(level_edit_screen_state::drag_drop_shape_type::type_target);
    targetShape.fixedShape = true;
    targetShape.position.x = targetPos.x;
    targetShape.position.y = targetPos.y;
    CreateDragDropPoints(targetPoints.cbegin(), targetPoints.cend(), std::back_inserter(targetShape.points));
    dragDropState->shapes.push_back(targetShape);
  }

  return dragDropState;
}

std::unique_ptr<game_level_data> CreateGameLevelData(const drag_drop_state& dragDropState)
{
  auto gameLevelData = std::make_unique<game_level_data>();

  for( const auto& shape : dragDropState.shapes )
  {
    switch( shape.type )
    {
      case level_edit_screen_state::drag_drop_shape_type::type_boundary:
        CreateGamePoints(shape.points.cbegin(), shape.points.cend(), std::back_inserter(gameLevelData->boundaryPoints));
        break;
      case level_edit_screen_state::drag_drop_shape_type::type_object:
        gameLevelData->objects.push_back(*CreateGameLevelObjectData(shape));
        break;
      case level_edit_screen_state::drag_drop_shape_type::type_player:
        gameLevelData->playerStartPosX = shape.position.x;
        gameLevelData->playerStartPosY = shape.position.y;
        break;
      case level_edit_screen_state::drag_drop_shape_type::type_target:
        gameLevelData->targets.push_back(game_point(shape.position.x, shape.position.y));
        break;
    }
  }

  return gameLevelData;
}

void CreateDragDropPoints(std::vector<game_point>::const_iterator begin, std::vector<game_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator)
{
  std::transform(begin, end, insertIterator, [](const game_point& point)
  {
    return drag_drop_point(point.x, point.y, drag_drop_point::type::type_real);
  });
}

void CreateDragDropPoints(std::list<game_point>::const_iterator begin, std::list<game_point>::const_iterator end, std::back_insert_iterator<std::list<drag_drop_point>> insertIterator)
{
  std::transform(begin, end, insertIterator, [](const game_point& point)
  {
    return drag_drop_point(point.x, point.y, drag_drop_point::type::type_real);
  });
}

std::unique_ptr<game_level_object_data> CreateGameLevelObjectData(const drag_drop_shape& dragDropShape)
{
  auto gameLevelObjectData = std::make_unique<game_level_object_data>();
  CreateGamePoints(dragDropShape.points.cbegin(), dragDropShape.points.cend(), std::back_inserter(gameLevelObjectData->points));
  return gameLevelObjectData;
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
