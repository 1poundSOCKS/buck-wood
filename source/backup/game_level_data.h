#pragma once

#ifdef INCLUDE_JSON

#include "geometry.h"
#include "render.h"
#include "json.h"

struct game_level_object_data
{
  std::vector<game_point> points;
};

struct game_level_data
{
  std::wstring filename;
  std::string name;
  float playerStartPosX = 0;
  float playerStartPosY = 0;
  int timeLimitInSeconds = 0;
  std::vector<game_point> boundaryPoints;
  std::vector<game_level_object_data> objects;
  std::vector<game_point> targets;
};

struct game_level_data_index
{
  game_level_data_index();
  game_level_data_index(const game_level_data_index& index);

  std::vector<std::unique_ptr<game_level_data>> gameLevelData;

  using const_iterator = std::vector<std::unique_ptr<game_level_data>>::const_iterator;
  using iterator = std::vector<std::unique_ptr<game_level_data>>::iterator;
};

struct game_level_data_filenames
{
  std::wstring GetNext();

  int filenameIndex = 0;
};

struct game_level_data_file_info
{
  game_level_data_file_info(const std::wstring& path, const std::wstring& name);

  std::wstring name;
  std::wstring path;
  std::wstring fullName;
};

struct game_level_data_files
{
  game_level_data_files(const std::wstring path);

  std::vector<game_level_data_file_info> fileInfo;
};

std::unique_ptr<game_level_data> LoadLevelDataFromJSON(const Json::Value& jsonObject, game_level_data& gameLevelData);
std::unique_ptr<game_level_object_data> LoadObjectDataFromJSON(const Json::Value& jsonObject);
std::unique_ptr<game_level_data> LoadGameLevelData(const std::wstring& dataPath, const std::wstring& file);
std::unique_ptr<game_level_data> LoadGameLevelData(const std::wstring& filename);
bool SaveGameLevelData(const game_level_data& gameLevelData);
std::unique_ptr<game_level_data_index> LoadAllGameLevelData(const std::wstring& dataPath);
bool SaveAllGameLevelData(const game_level_data_index& gameLevelDataIndex);
std::string SaveJsonDataToString(Json::Value& root);
[[nodiscard]] auto GetGameLevelBoundary(const game_level_data& levelData) -> game_rect;

void LoadLevelObjects(const game_level_data& levelData, auto objectInserter)
{
  std::transform(levelData.objects.cbegin(), levelData.objects.cend(), objectInserter,
  [](const game_level_object_data& objectData) -> game_closed_object
  {
    game_closed_object object(objectData.points.cbegin(), objectData.points.cend());
    return object;
  });
}

#endif
