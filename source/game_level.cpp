#include "game_level.h"
#include "game_level_file.h"

game_level::game_level(const game_level_data& gameLevelData)
{
  width = gameLevelData.width;
  height = gameLevelData.height;
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

bool OutOfGameLevelBoundary(const game_level& gameLevel, float x, float y)
{
  return ( x < 0 || y < 0 || x > gameLevel.width || y > gameLevel.height );
}

game_level_data_ptr CreateFirstGameLevelData()
{
  game_level_data_ptr gameLevelData = std::make_shared<game_level_data>();

  gameLevelData->width = 2000.0f;
  gameLevelData->height = 2200.0f;
  gameLevelData->playerStartPosX = 1000.0f;
  gameLevelData->playerStartPosY = 200.0f;
  gameLevelData->timeLimitInSeconds = 20;

  const game_point boundaryPoints[] = {
    game_point(20, 20),
    game_point(1980, 20),
    game_point(1980, 980),
    game_point(1300, 980),
    game_point(1200, 900),
    game_point(800, 700),
    game_point(20, 980)
  };

  const int boundaryPointCount = sizeof(boundaryPoints) / sizeof(game_point);

  gameLevelData->boundaryPoints.insert(gameLevelData->boundaryPoints.end(), &boundaryPoints[0], &boundaryPoints[boundaryPointCount]);

  const game_point targetPositions[] = {
    game_point(1200.0f, 700.0f)
  };

  const int targetCount = sizeof(targetPositions) / sizeof(game_point);

  gameLevelData->targets.insert(gameLevelData->targets.end(), &targetPositions[0], &targetPositions[targetCount]);

  return gameLevelData;
}

game_level_data_ptr CreateSecondGameLevelData()
{
  game_level_data_ptr gameLevelData = std::make_shared<game_level_data>();

  gameLevelData->width = 2000.0f;
  gameLevelData->height = 2200.0f;
  gameLevelData->playerStartPosX = 1000.0f;
  gameLevelData->playerStartPosY = 200.0f;
  gameLevelData->timeLimitInSeconds = 30;

  const game_point boundaryPoints[] = {
    game_point(20, 20),
    game_point(1980, 20),
    game_point(1980, 980),
    game_point(1300, 980),
    game_point(1400, 2000),
    game_point(1300, 2100),
    game_point(1100, 2000),
    game_point(1200, 900),
    game_point(800, 700),
    game_point(20, 980)
  };

  const int boundaryPointCount = sizeof(boundaryPoints) / sizeof(game_point);

  gameLevelData->boundaryPoints.insert(gameLevelData->boundaryPoints.end(), &boundaryPoints[0], &boundaryPoints[boundaryPointCount]);

  const game_point object1Points[] = {
    game_point(1600, 700),
    game_point(1700, 750),
    game_point(1700, 800),
    game_point(1750, 750),
    game_point(1700, 600)
  };

  const int object1PointCount = sizeof(object1Points) / sizeof(game_point);

  game_level_object_data_ptr object1Data = std::make_unique<game_level_object_data>();
  object1Data->insert(object1Data->end(), &object1Points[0], &object1Points[object1PointCount]);
  gameLevelData->objects.push_back(std::move(object1Data));

  const game_point object2Points[] = {
    game_point(300, 200),
    game_point(400, 350),
    game_point(400, 600),
    game_point(350, 550),
    game_point(320, 400)
  };

  const int object2PointCount = sizeof(object2Points) / sizeof(game_point);

  game_level_object_data_ptr object2Data = std::make_unique<game_level_object_data>();
  object2Data->insert(object2Data->end(), &object2Points[0], &object2Points[object2PointCount]);
  gameLevelData->objects.push_back(std::move(object2Data));

  const game_point targetPositions[] = {
    game_point(1250.0f, 1950.0f),
    game_point(100.0f, 300.0f),
    game_point(1800.0f, 900.0f)
  };

  const int targetCount = sizeof(targetPositions) / sizeof(game_point);

  gameLevelData->targets.insert(gameLevelData->targets.end(), &targetPositions[0], &targetPositions[targetCount]);

  return gameLevelData;
}

game_level_data_ptr CreateThirdGameLevelData()
{
  game_level_data_ptr gameLevelData = std::make_shared<game_level_data>();

  gameLevelData->width = 2000.0f;
  gameLevelData->height = 2200.0f;
  gameLevelData->playerStartPosX = 1000.0f;
  gameLevelData->playerStartPosY = 200.0f;
  gameLevelData->timeLimitInSeconds = 30;

  const game_point boundaryPoints[] = {
    game_point(20, 20),
    game_point(1980, 20),
    game_point(1980, 980),
    game_point(1300, 980),
    game_point(1400, 2000),
    game_point(1300, 2100),
    game_point(900, 2100),
    game_point(1100, 2000),
    game_point(1200, 900),
    game_point(800, 700),
    game_point(20, 980)
  };

  const int boundaryPointCount = sizeof(boundaryPoints) / sizeof(game_point);

  gameLevelData->boundaryPoints.insert(gameLevelData->boundaryPoints.end(), &boundaryPoints[0], &boundaryPoints[boundaryPointCount]);

  const game_point object1Points[] = {
    game_point(1600, 700),
    game_point(1700, 750),
    game_point(1700, 800),
    game_point(1750, 750),
    game_point(1700, 600)
  };

  const int object1PointCount = sizeof(object1Points) / sizeof(game_point);

  game_level_object_data_ptr object1Data = std::make_unique<game_level_object_data>();
  object1Data->insert(object1Data->end(), &object1Points[0], &object1Points[object1PointCount]);
  gameLevelData->objects.push_back(std::move(object1Data));

  const game_point object2Points[] = {
    game_point(300, 200),
    game_point(400, 350),
    game_point(400, 600),
    game_point(350, 550),
    game_point(320, 400)
  };

  const int object2PointCount = sizeof(object2Points) / sizeof(game_point);

  game_level_object_data_ptr object2Data = std::make_unique<game_level_object_data>();
  object2Data->insert(object2Data->end(), &object2Points[0], &object2Points[object2PointCount]);
  gameLevelData->objects.push_back(std::move(object2Data));

  const game_point targetPositions[] = {
    game_point(1250.0f, 1950.0f),
    game_point(100.0f, 300.0f),
    game_point(1800.0f, 900.0f)
  };

  const int targetCount = sizeof(targetPositions) / sizeof(game_point);

  gameLevelData->targets.insert(gameLevelData->targets.end(), &targetPositions[0], &targetPositions[targetCount]);

  return gameLevelData;
}

game_level_data_ptr LoadLevelDataFromJSON(const level_data_json& jsonData)
{
  game_level_data_ptr levelData = std::make_shared<game_level_data>();

  levelData->width = jsonData.width;
  levelData->height = jsonData.height;
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
