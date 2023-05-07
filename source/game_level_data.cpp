#include "pch.h"
#include "game_level_data.h"

#pragma comment(lib,"jsoncpp.lib")

Json::Value FormatAsJson(const game_level_object_data& object);
template<class T> Json::Value FormatAsJson(const std::vector<T>& items);

game_level_data_index::game_level_data_index()
{
}

game_level_data_index::game_level_data_index(const game_level_data_index& index)
{
  std::transform(index.gameLevelData.cbegin(), index.gameLevelData.cend(), std::back_inserter(gameLevelData), [](const auto& data)
  {
    return std::make_unique<game_level_data>(*data);
  });
}

game_level_data_file_info::game_level_data_file_info(const std::wstring& path, const std::wstring& name)
: path(path), name(name)
{
  std::filesystem::path tmpName = path;
  tmpName /= name;
  fullName = tmpName;
}

game_level_data_files::game_level_data_files(const std::wstring path)
{
  game_level_data_filenames filenames;

  bool fileExists = true;

  while( fileExists )
  {
    std::wstring name = filenames.GetNext();
    game_level_data_file_info info(path, name);
    fileExists = std::filesystem::exists(info.fullName);
    if( fileExists ) fileInfo.push_back(info);
  }
}

std::unique_ptr<game_level_data> LoadLevelDataFromJSON(const Json::Value& jsonObject)
{
  auto levelData = std::make_unique<game_level_data>();
  if( !jsonObject )
  {
    levelData->timeLimitInSeconds = 30;
    levelData->boundaryPoints.push_back(game_point(100, 0));
    levelData->boundaryPoints.push_back(game_point(100, 400));
    levelData->boundaryPoints.push_back(game_point(-100, 400));
    levelData->boundaryPoints.push_back(game_point(-100, 0));
    return levelData;
  }

  levelData->name = jsonObject["name"].asCString();
  levelData->playerStartPosX = jsonObject["playerStartPosX"].asFloat();
  levelData->playerStartPosY = jsonObject["playerStartPosY"].asFloat();
  levelData->timeLimitInSeconds = jsonObject["timeLimitInSeconds"].asInt();

  Json::Value boundaryPoints = jsonObject["boundaryPoints"];
  Json::ArrayIndex boundaryPointCount = boundaryPoints.size();

  for( auto i = 0u; i < boundaryPointCount; ++i )
  {
    Json::Value boundaryPoint = boundaryPoints[i];
    auto x = boundaryPoint["x"].asFloat();
    auto y = boundaryPoint["y"].asFloat();
    game_point point { x, y };
    levelData->boundaryPoints.push_back(point);
  }

  Json::Value objects = jsonObject["objects"];
  Json::ArrayIndex objectCount = objects.size();

  levelData->objects.reserve(objectCount);

  for( auto i = 0u; i < objectCount; i++)
  {
    Json::Value objectValue = objects[i];
    auto objectDataPtr = LoadObjectDataFromJSON(objectValue);
    levelData->objects.push_back(*objectDataPtr);
  }

  Json::Value targets = jsonObject["targets"];
  Json::ArrayIndex targetCount = targets.size();

  for( auto i = 0u; i < targetCount; i++)
  {
    Json::Value target = targets[i];
    auto x = target["x"].asFloat();
    auto y = target["y"].asFloat();
    levelData->targets.push_back( game_point { x, y } );
  }

  return levelData;
}

std::unique_ptr<game_level_object_data> LoadObjectDataFromJSON(const Json::Value& jsonObject)
{
  auto objectDataPtr = std::make_unique<game_level_object_data>();

  Json::Value objectPoints = jsonObject["points"];
  Json::ArrayIndex objectPointCount = objectPoints.size();
  
  objectDataPtr->points.reserve(objectPointCount);
  
  for( auto i = 0u; i < objectPointCount;  i++ )
  {
    Json::Value objectPoint = objectPoints[i];
    auto x = objectPoint["x"].asFloat();
    auto y = objectPoint["y"].asFloat();
    game_point point(x, y);
    objectDataPtr->points.push_back(point);
  }

  return objectDataPtr;
}

std::unique_ptr<game_level_data> LoadGameLevelData(const std::wstring& dataPath, const std::wstring& file)
{
  std::filesystem::path fullFilename = dataPath;
  fullFilename /= L"levels";
  fullFilename /= file;

  std::ifstream ifs(fullFilename);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);
  auto gameLevelData = LoadLevelDataFromJSON(root);
  gameLevelData->filename = fullFilename;
  return gameLevelData;
}

std::unique_ptr<game_level_data> LoadGameLevelData(const std::wstring& filename)
{
  std::ifstream ifs(filename);
  Json::Reader reader;
  Json::Value root;
  reader.parse(ifs, root);
  auto gameLevelData = LoadLevelDataFromJSON(root);
  gameLevelData->filename = filename;
  return gameLevelData;
}

Json::Value FormatAsJson(const game_point& point)
{
    Json::Value root;
    root["x"] = point.x;
    root["y"] = point.y;
    return root;
}

Json::Value FormatAsJson(const game_level_object_data& object)
{
  Json::Value root;  
  root["points"] = FormatAsJson(object.points);
  return root;
}

template<class T>
Json::Value FormatAsJson(const std::vector<T>& items)
{
  Json::Value root = Json::arrayValue;

  std::vector<Json::Value> itemsJson;
  std::transform(items.cbegin(), items.end(), std::back_inserter(itemsJson), [](const T& item)
    {
      return FormatAsJson(item);
    }
  );

  for( const auto& item : itemsJson ) { root.append(item); }

  return root;
}

bool SaveGameLevelData(const game_level_data& gameLevelData)
{
  Json::Value root;

  root["name"] = gameLevelData.name;
  root["playerStartPosX"] = gameLevelData.playerStartPosX;
  root["playerStartPosY"] = gameLevelData.playerStartPosY;
  root["timeLimitInSeconds"] = gameLevelData.timeLimitInSeconds;
  root["boundaryPoints"] = FormatAsJson(gameLevelData.boundaryPoints);
  root["objects"] = FormatAsJson(gameLevelData.objects);
  root["targets"] = FormatAsJson(gameLevelData.targets);

  std::string jsonData = SaveJsonDataToString(root);

  std::filesystem::path saveFilename = gameLevelData.filename;
  std::filesystem::path backupFilename = saveFilename.parent_path();
  backupFilename /= "bak";
  backupFilename /= saveFilename.filename();

  // TODO: add backup

  std::ofstream ofs(gameLevelData.filename);
  ofs.write(jsonData.c_str(), jsonData.length());
  bool saved = ofs.good();
  ofs.close();
  return saved;
}

std::string SaveJsonDataToString(Json::Value& root)
{
  Json::StyledWriter styledWriter;
  return styledWriter.write(root);
}

std::unique_ptr<game_level_data_index> LoadAllGameLevelData(const std::wstring& dataPath)
{
  auto gameLevelDataIndex = std::make_unique<game_level_data_index>();
  std::filesystem::path path = dataPath;
  path /= L"levels";

  game_level_data_files files(path);

  std::transform(files.fileInfo.cbegin(), files.fileInfo.cend(), std::back_inserter(gameLevelDataIndex->gameLevelData), [](const auto& info)
  {
    return LoadGameLevelData(info.fullName);
  });

  return gameLevelDataIndex;
}

bool SaveAllGameLevelData(const game_level_data_index& gameLevelDataIndex)
{
  std::vector<bool> saveLevelDataReturn;
  std::transform(gameLevelDataIndex.gameLevelData.cbegin(), gameLevelDataIndex.gameLevelData.end(), std::back_inserter(saveLevelDataReturn),
    [](const std::unique_ptr<game_level_data>& gameLevelData)
    {
      return SaveGameLevelData(*gameLevelData);
    }
  );

  bool allSaved = std::reduce(saveLevelDataReturn.cbegin(), saveLevelDataReturn.cend(), true,
    [](auto left, auto right)
    {
      return left && right;
    }
  );

  return allSaved;
}

std::wstring game_level_data_filenames::GetNext()
{
  return std::format(L"level_{:03}.json", ++filenameIndex);
}

[[nodiscard]] auto GetGameLevelBoundary(const game_level_data& levelData) -> game_rect
{
  return std::reduce(
    levelData.boundaryPoints.cbegin(), levelData.boundaryPoints.cend(), 
    game_rect { 0, 0, 0, 0 },
    [](auto boundary, auto point) -> game_rect
    {
      return {
        min(boundary.topLeft.x, point.x),
        min(boundary.topLeft.y, point.y),
        max(boundary.bottomRight.x, point.x),
        max(boundary.bottomRight.y, point.y)
      };
    }
  );
}
