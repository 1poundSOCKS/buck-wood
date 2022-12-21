#include "pch.h"
#include "level_edit_screen_state.h"
#include "drag_drop.h"
#include "game_objects.h"
#include "screen_view.h"

D2D1::Matrix3x2F CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, const level_edit_screen_state& screenState);
void UpdateScreenExitState(level_edit_screen_state& screenState, const screen_input_state& screenInputState);
void UpdateScreenStateMouseData(level_edit_screen_state& screenState, const screen_input_state& screenInputState);
void RunDragDrop(level_edit_screen_state& screenState, const screen_input_state& screenInputState);
void LoadCurrentLevel(level_edit_screen_state& screenState);
void SaveCurrentLevel(level_edit_screen_state& screenState);
float GetScreenUpdateIntervalInSeconds(const level_edit_screen_state& screenState);
void ScrollView(level_edit_screen_state& screenState, const screen_input_state& screenInputState);
std::unique_ptr<game_level_data> CreateGameLevelData(const drag_drop_state& dragDropState);
std::unique_ptr<game_level_object_data> CreateGameLevelObjectData(const drag_drop_shape& dragDropShape);

std::unique_ptr<drag_drop_state> CreateDragDropState(const game_level_data& gameLevelData);
void AddDragDropShapeForTarget(drag_drop_state& dragDropState, const game_point& targetPostion);
constexpr std::array<game_point, 3> GetDefaultObjectShape();
void AddDragDropShapeForObject(drag_drop_state& dragDropState, const game_point& targetPosition);
void CreateGamePoints(std::list<drag_drop_point>::const_iterator begin, std::list<drag_drop_point>::const_iterator end, std::back_insert_iterator<std::vector<game_point>> gamePointInserter);

level_edit_screen_state::level_edit_screen_state(const game_level_data_index& gameLevelDataIndex, const screen_render_data renderData)
: gameLevelDataIndex(gameLevelDataIndex), renderData(renderData)
{
  timer.frequency = performance_counter::QueryFrequency();
  timer.initialValue = timer.currentValue = performance_counter::QueryValue();

  currentLevelDataIterator = this->gameLevelDataIndex.gameLevelData.begin();
  LoadCurrentLevel(*this);
}

void UpdateScreenState(level_edit_screen_state& screenState, const screen_input_state& screenInputState)
{
  screenState.previousTimerValue = screenState.timer.currentValue;
  screenState.timer.currentValue = performance_counter::QueryValue();
  UpdateScreenStateMouseData(screenState, screenInputState);

  if( screenState.viewState == level_edit_screen_state::view_exit )
  {
    UpdateScreenExitState(screenState, screenInputState);
  }
  else if( KeyPressed(screenInputState, DIK_ESCAPE) )
  {
    SaveCurrentLevel(screenState);
    screenState.viewState = level_edit_screen_state::view_exit;
  }
  else if( KeyPressed(screenInputState, DIK_PGUP) )
  {
    auto nextLevelData = std::next(screenState.currentLevelDataIterator);
    
    if( nextLevelData != screenState.gameLevelDataIndex.gameLevelData.end() )
    {
      SaveCurrentLevel(screenState);
      screenState.currentLevelDataIterator = nextLevelData;
      LoadCurrentLevel(screenState);
    }
  }
  else if( KeyPressed(screenInputState, DIK_PGDN) )
  {
    if( screenState.currentLevelDataIterator != screenState.gameLevelDataIndex.gameLevelData.begin() )
    {
      SaveCurrentLevel(screenState);
      screenState.currentLevelDataIterator--;
      LoadCurrentLevel(screenState);
    }
  }
  else if( KeyPressed(screenInputState, DIK_T) )
  {
    AddDragDropShapeForTarget(*screenState.dragDropState, {screenState.levelMouseX, screenState.levelMouseY});
  }
  else if( KeyPressed(screenInputState, DIK_O) )
  {
    AddDragDropShapeForObject(*screenState.dragDropState, {screenState.levelMouseX, screenState.levelMouseY});
  }
  else
  {
    ScrollView(screenState, screenInputState);
    RunDragDrop(screenState, screenInputState);
  }
}

bool ContinueRunning(const level_edit_screen_state& screenState)
{
  return !screenState.returnToMenu;
}

void UpdateScreenExitState(level_edit_screen_state& screenState, const screen_input_state& screenInputState)
{
  if( KeyPressed(screenInputState, DIK_ESCAPE) )
  {
    screenState.viewState = level_edit_screen_state::view_default;
  }
  else if( KeyPressed(screenInputState, DIK_N) )
  {
    screenState.saveChanges = false;
    screenState.returnToMenu = true;
  }
  else if( KeyPressed(screenInputState, DIK_Y) )
  {
    screenState.saveChanges = true;
    screenState.returnToMenu = true;
  }
}

void UpdateScreenStateMouseData(level_edit_screen_state& screenState, const screen_input_state& screenInputState)
{
  screenState.renderTargetMouseData = screenInputState.renderTargetMouseData;

  static const float scale = 0.8f;

  screenState.viewTransform = CreateGameLevelTransform(
    screenState.levelCenterX, 
    screenState.levelCenterY, 
    scale, 
    screenInputState.renderTargetMouseData.size.width, 
    screenInputState.renderTargetMouseData.size.height
  );

  auto mouseTransform = screenState.viewTransform;

  if( mouseTransform.Invert() )
  {
    D2D1_POINT_2F inPoint;
    inPoint.x = screenState.renderTargetMouseData.x;
    inPoint.y = screenState.renderTargetMouseData.y;
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

void RunDragDrop(level_edit_screen_state& screenState, const screen_input_state& screenInputState)
{
  drag_drop_control_state dragDropControlState
  {
    screenInputState.windowData.mouse.leftButtonDown,
    LeftMouseButtonDrag(screenInputState),
    LeftMouseButtonReleased(screenInputState),
    screenState.levelMouseX,
    screenState.levelMouseY,
    KeyPressed(screenInputState, DIK_DELETE)
  };

  ProcessDragDrop(*screenState.dragDropState, dragDropControlState);
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
  boundaryShape.closed = false;
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
  auto playerGeometryData = GetPlayerGeometryData();
  CreateDragDropPoints(playerGeometryData.cbegin(), playerGeometryData.cend(), std::back_inserter(playerShape.points));
  dragDropState->shapes.push_back(playerShape);

  for( const auto& targetPos : gameLevelData.targets )
  {
    AddDragDropShapeForTarget(*dragDropState, {targetPos.x, targetPos.y});
  }

  return dragDropState;
}

void AddDragDropShapeForObject(drag_drop_state& dragDropState, const game_point& targetPosition)
{
    drag_drop_shape dragDropObjectShape(level_edit_screen_state::drag_drop_shape_type::type_object);
    std::vector<game_point> objectShape;
    const auto defaultObjectShape = GetDefaultObjectShape();
    std::transform(defaultObjectShape.cbegin(), defaultObjectShape.cend(), std::back_inserter(objectShape), [targetPosition](const auto& point)
    {
      return game_point(targetPosition.x + point.x, targetPosition.y + point.y);
    });
    CreateDragDropPoints(objectShape.cbegin(), objectShape.cend(), std::back_inserter(dragDropObjectShape.points));
    dragDropState.shapes.push_back(dragDropObjectShape);
}

void AddDragDropShapeForTarget(drag_drop_state& dragDropState, const game_point& targetPostion)
{
  auto targetGeometryData = GetDefaultTargetGeometryData();
  drag_drop_shape targetShape(level_edit_screen_state::drag_drop_shape_type::type_target);
  targetShape.fixedShape = true;
  targetShape.position.x = targetPostion.x;
  targetShape.position.y = targetPostion.y;
  CreateDragDropPoints(targetGeometryData.cbegin(), targetGeometryData.cend(), std::back_inserter(targetShape.points));
  dragDropState.shapes.push_back(targetShape);
}

constexpr std::array<game_point, 3> GetDefaultObjectShape()
{
  const float objectSize = 100.0;
  return std::array<game_point, 3>({game_point(0, -objectSize), game_point(objectSize, objectSize), game_point(-objectSize, objectSize)});
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

D2D1::Matrix3x2F CreateViewTransform(const D2D1_SIZE_F& renderTargetSize, const level_edit_screen_state& screenState)
{
  static const float scale = 0.8f;
  return CreateGameLevelTransform(screenState.levelCenterX, screenState.levelCenterY, scale, renderTargetSize.width, renderTargetSize.height);
}

float GetScreenUpdateIntervalInSeconds(const level_edit_screen_state& screenState)
{
  return
    static_cast<float>(screenState.timer.currentValue - screenState.previousTimerValue) / 
    static_cast<float>(screenState.timer.frequency);
}

void ScrollView(level_edit_screen_state& screenState, const screen_input_state& screenInputState)
{
  float intervalTime = GetScreenUpdateIntervalInSeconds(screenState);
  float scrollDistance = 500.0 * intervalTime;

  float ratioMouseX = GetRatioMouseX(screenInputState);
  float ratioMouseY = GetRatioMouseY(screenInputState);

  if( ratioMouseX < 0.1f ) screenState.levelCenterX -= scrollDistance;
  else if( ratioMouseX > 0.9f ) screenState.levelCenterX += scrollDistance;

  if( ratioMouseY < 0.1f ) screenState.levelCenterY -= scrollDistance;
  else if( ratioMouseY > 0.9f ) screenState.levelCenterY += scrollDistance;
}
