#include "game_objects.h"
#include <fstream>

game_line::game_line(const game_point& start, const game_point& end) : start(start), end(end)
{
}

game_line::game_line(float startX, float startY, float endX, float endY) : start(startX, startY), end(endX, endY)
{
}

game_shape::game_shape()
{
}

game_shape::game_shape(const game_point* points, int pointCount)
{
  InitializeShape(points, pointCount, *this);
}

game_shape::game_shape(const std::vector<game_point>& pointsToCopy)
{
  std::copy( pointsToCopy.begin(), pointsToCopy.end(), std::back_inserter(points) );
  CreateShapeLinesFromPoints(lines, points);
}

game_shape::game_shape(const game_level_object_data& objectData)
{
  for( const auto& point: objectData.points )
  {
    points.push_back(game_point(objectData.x + point.x, objectData.y + point.y));
  }

  CreateShapeLinesFromPoints(lines, points);
 }

mouse_cursor::mouse_cursor() : xPos(0), yPos(0)
{
  static const float cursorSize = 20.0f;
  static const float cursorSizeGap = 10.0f;

  lines.push_back(game_line(0,-cursorSize,0,-cursorSizeGap));
  lines.push_back(game_line(0,cursorSize,0,cursorSizeGap));
  lines.push_back(game_line(-cursorSize,0,-cursorSizeGap,0));
  lines.push_back(game_line(cursorSize,0,cursorSizeGap,0));
}

player_ship::player_ship() : xPos(0), yPos(0), xVelocity(0), yVelocity(0), angle(0)
{
  static const game_point points[] = {
    game_point(0, -10),
    game_point(7, 10),
    game_point(-7, 10)
  };

  static const int pointCount = sizeof(points) / sizeof(game_point);

  outline = std::make_unique<game_shape>(points, pointCount);
  thruster = std::make_unique<game_line>(game_point(5, 14), game_point(-5, 14));
}

bullet::bullet(float x, float y, float range) : startX(x), startY(y), xPos(x), yPos(y), range(range), xVelocity(0), yVelocity(0), angle(0), outsideLevel(false)
{
}

target::target(float x, float y, float size) : state(DEACTIVATED)
{
  float halfSize = size / 2;

  const game_point points[] = {
    game_point(x, y - halfSize ),
    game_point(x + halfSize, y),
    game_point(x, y + halfSize),
    game_point(x - halfSize, y)
  };

  static const int pointCount = sizeof(points) / sizeof(game_point);

  InitializeShape(points, pointCount, shape);
}

game_level::game_level(const game_level_data& gameLevelData)
{
  playerStartPosX = gameLevelData.playerStartPosX;
  playerStartPosY = gameLevelData.playerStartPosY;
  timeLimitInSeconds = gameLevelData.timeLimitInSeconds;

  boundary = std::make_unique<game_shape>(gameLevelData.boundaryPoints);

  game_level_object_data_ptr objectData = std::make_unique<game_level_object_data>();
  for( const auto& object: gameLevelData.objects )
  {
    game_shape_ptr gameShape = std::make_unique<game_shape>(*object);
    objects.push_back(std::move(gameShape));
  }
  
  for( const auto& t: gameLevelData.targets )
  {
    target_ptr levelTarget = std::make_unique<target>(t.x, t.y, 40.0f);
    targets.push_back(std::move(levelTarget));
  }
}

level_data_json::level_data_json(const wchar_t* filename)
{
  std::ifstream ifs(filename);
  Json::Reader reader;
  reader.parse(ifs, root);
  name = root["name"].asString();
  width = root["width"].asInt();
  height = root["height"].asInt();
  playerStartPosX = root["playerStartPosX"].asInt();
  playerStartPosY = root["playerStartPosY"].asInt();
  timeLimitInSeconds = root["timeLimitInSeconds"].asInt();
  boundaryPoints = root["boundaryPoints"];
  boundaryPointCount = boundaryPoints.size();
  objects = root["objects"];
  objectCount = objects.size();
  targets = root["targets"];
  targetCount = targets.size();
}

std::unique_ptr<player_ship> CreatePlayerShip()
{
  return std::make_unique<player_ship>();
}

void InitializeShape(const game_point* points, int pointCount, game_shape& shape)
{
  for( int i = 0; i < pointCount; i++)
  {
    shape.points.push_back(points[i]);
    int endPointIndex = (i + 1) % pointCount;
    shape.lines.push_back(game_line(points[i].x, points[i].y, points[endPointIndex].x, points[endPointIndex].y));
  }
}

void CreateShapeLinesFromPoints(std::list<game_line>& lines, const std::list<game_point>& points)
{
  std::list<game_point>::const_iterator i = points.begin();
  
  while( i != points.end()  )
  {
    const game_point& point1 = *i;
    i++;
    if( i != points.end() )
    {
      const game_point& point2 = *i;
      lines.push_back(game_line(point1.x, point1.y, point2.x, point2.y));
    }
    else
    {
      const game_point& point2 = *points.begin();
      lines.push_back(game_line(point1.x, point1.y, point2.x, point2.y));
    }
  }
}

game_level_data_ptr LoadLevelDataFromJSON(const level_data_json& jsonData)
{
  game_level_data_ptr levelData = std::make_shared<game_level_data>();

  levelData->playerStartPosX = jsonData.playerStartPosX;
  levelData->playerStartPosY = jsonData.playerStartPosY;
  levelData->timeLimitInSeconds = jsonData.timeLimitInSeconds;

  for( int i = 0; i < jsonData.boundaryPointCount; i++)
  {
    Json::Value boundaryPoint = jsonData.boundaryPoints[i];
    int x = boundaryPoint["x"].asInt();
    int y = boundaryPoint["y"].asInt();
    game_point point(x, y);
    levelData->boundaryPoints.push_back(point);
  }

  levelData->objects.reserve(jsonData.objectCount);

  for( int i = 0; i < jsonData.objectCount; i++)
  {
    Json::Value jsonObject = jsonData.objects[i];
    auto objectDataPtr = LoadLevelObjectDataFromJSON(jsonObject);
    levelData->objects.push_back(std::move(objectDataPtr));
  }

  for( int i = 0; i < jsonData.targetCount; i++)
  {
    Json::Value target = jsonData.targets[i];
    int x = target["x"].asInt();
    int y = target["y"].asInt();
    game_point point(x, y);
    levelData->targets.push_back(point);
  }

  return levelData;
}

game_level_object_data_ptr LoadLevelObjectDataFromJSON(const Json::Value& jsonObject)
{
  game_level_object_data_ptr objectDataPtr = std::make_unique<game_level_object_data>();

  objectDataPtr->x = jsonObject["x"].asFloat();
  objectDataPtr->y = jsonObject["x"].asFloat();

  Json::Value objectPoints = jsonObject["points"];
  Json::ArrayIndex objectPointCount = objectPoints.size();
  
  objectDataPtr->points.reserve(objectPointCount);
  
  for( int i = 0; i < objectPointCount;  i++ )
  {
    Json::Value objectPoint = objectPoints[i];
    int x = objectPoint["x"].asInt();
    int y = objectPoint["y"].asInt();
    game_point point(x, y);
    objectDataPtr->points.push_back(point);
  }

  return objectDataPtr;
}
