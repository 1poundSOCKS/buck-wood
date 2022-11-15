#ifndef _game_object_
#define _game_object_

#include <memory>
#include <vector>
#include <list>
#include <iterator>
#include "json.h"

struct game_point
{
  game_point(float x, float y) : x(x), y(y) {}

  float x = 0, y = 0;
};

struct game_point_and_next_point
{
  game_point_and_next_point(const game_point& point, const game_point& nextPoint);

  const game_point& point;
  const game_point& nextPoint;
};

struct game_line
{
  game_line(const game_point& start, const game_point& end);
  game_line(float startX, float startY, float endX, float endY);

  game_point start, end;
};

struct game_level_object_data
{
  std::vector<game_point> points;
};

struct game_shape
{
  game_shape();
  game_shape(const game_point* points, int pointCount);
  game_shape(const std::vector<game_point>& pointsToCopy);
  game_shape(const game_level_object_data& objectData);

  std::list<game_point> points;
  std::list<game_line> lines;
};

struct mouse_cursor
{
  mouse_cursor();

  std::list<game_line> lines;
};

struct player_ship
{
  player_ship();

  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  std::unique_ptr<game_shape> outline;
  std::unique_ptr<game_line> thruster;
  bool thrusterOn = false;
};

struct bullet
{
  bullet(float x, float y, float range);

  float startX, startY;
  float xPos, yPos;
  float xVelocity, yVelocity;
  float angle;
  float range;
  bool outsideLevel;
};

struct target
{
  target(float x, float y, float size);

  enum STATE { DEACTIVATED, ACTIVATED };

  STATE state;
  game_shape shape;
};

struct game_level_data
{
  std::wstring filename;
  std::string name;
  float playerStartPosX = 0, playerStartPosY = 0;
  int timeLimitInSeconds = 0;
  std::vector<game_point> boundaryPoints;
  std::vector<game_level_object_data> objects;
  std::vector<game_point> targets;
};

struct game_level
{
  game_level(const game_level_data& gameLevelData);

  std::string name;
  float playerStartPosX = 0, playerStartPosY = 0;
  float timeLimitInSeconds = 0;
  std::unique_ptr<game_shape> boundary;
  std::list<std::unique_ptr<game_shape>> objects;
  std::list<std::unique_ptr<target>> targets;
};

struct game_level_data_index
{
  game_level_data_index();
  game_level_data_index(const game_level_data_index& index);

  std::vector<std::unique_ptr<game_level_data>> gameLevelData;
};

std::unique_ptr<player_ship> CreatePlayerShip();

void InitializeShape(const game_point* points, int pointCount, game_shape& boundary);
void InitializeShape(std::list<game_point> points, game_shape& shape);

void CreateShapeLinesFromPoints(std::list<game_line>& lines, const std::list<game_point>& points);
void InitializeTargetShape(float x, float y, float size, game_shape& shape);

std::unique_ptr<game_level_data> LoadLevelDataFromJSON(const Json::Value& jsonObject, game_level_data& gameLevelData);
std::unique_ptr<game_level_object_data> LoadObjectDataFromJSON(const Json::Value& jsonObject);

std::unique_ptr<game_level_data> LoadGameLevelData(const std::wstring& dataPath, const std::wstring& file);
bool SaveGameLevelData(const game_level_data& gameLevelData);

std::unique_ptr<game_level_data_index> LoadAllGameLevelData(const std::wstring& dataPath);
bool SaveAllGameLevelData(const game_level_data_index& gameLevelDataIndex);

std::string SaveJsonDataToString(Json::Value& root);

#endif
