#include "game_objects.h"
#include <fstream>
#include <numeric>
#include <format>
#include "framework/data_files.h"

Json::Value FormatAsJson(const game_level_object_data& object);
template<class T> Json::Value FormatAsJson(const std::vector<T>& items);

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
  std::copy(objectData.points.cbegin(), objectData.points.end(), std::back_inserter(points));
  CreateShapeLinesFromPoints(lines, points);
}

mouse_cursor::mouse_cursor()
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
  InitializeTargetShape(x, y, size, shape);
}

game_level::game_level(const game_level_data& gameLevelData)
: name(gameLevelData.name),
  playerStartPosX(gameLevelData.playerStartPosX),
  playerStartPosY(gameLevelData.playerStartPosY),
  timeLimitInSeconds(gameLevelData.timeLimitInSeconds)
{
  boundary = std::make_unique<game_shape>(gameLevelData.boundaryPoints);

  std::unique_ptr<game_level_object_data> objectData = std::make_unique<game_level_object_data>();
  for( const auto& object: gameLevelData.objects )
  {
    std::unique_ptr<game_shape> gameShape = std::make_unique<game_shape>(object);
    objects.push_back(std::move(gameShape));
  }
  
  for( const auto& t: gameLevelData.targets )
  {
    auto levelTarget = std::make_unique<target>(t.x, t.y, 40.0f);
    targets.push_back(std::move(levelTarget));
  }
}

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

// game_level_data_file_reader::game_level_data_file_reader(const std::wstring& path, const std::wstring& filename)
// : path(path), filename(filename)
// {
//   fullFilename = GetFullLevelFilename(path, filename);
//   inputStream.open(fullFilename);
// }

// game_level_data_file_readers::game_level_data_file_readers(const std::wstring& dataPath)
// {
//   game_level_data_filenames filenames;


// }

game_level_data_file_info::game_level_data_file_info(const std::wstring& path, const std::wstring& name)
: path(path), name(name)
{
  fs::path tmpName = path;
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
    fileExists = fs::exists(info.fullName);
    if( fileExists ) fileInfo.push_back(info);
  }
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

void InitializeTargetShape(float x, float y, float size, game_shape& shape)
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

std::unique_ptr<game_level_data> LoadLevelDataFromJSON(const Json::Value& jsonObject)
{
  auto levelData = std::make_unique<game_level_data>();

  levelData->name = jsonObject["name"].asCString();
  levelData->playerStartPosX = jsonObject["playerStartPosX"].asInt();
  levelData->playerStartPosY = jsonObject["playerStartPosY"].asInt();;
  levelData->timeLimitInSeconds = jsonObject["timeLimitInSeconds"].asInt();;

  Json::Value boundaryPoints = jsonObject["boundaryPoints"];
  Json::ArrayIndex boundaryPointCount = boundaryPoints.size();

  for( int i = 0; i < boundaryPointCount; i++)
  {
    Json::Value boundaryPoint = boundaryPoints[i];
    int x = boundaryPoint["x"].asInt();
    int y = boundaryPoint["y"].asInt();
    game_point point(x, y);
    levelData->boundaryPoints.push_back(point);
  }

  Json::Value objects = jsonObject["objects"];
  Json::ArrayIndex objectCount = objects.size();

  levelData->objects.reserve(objectCount);

  for( int i = 0; i < objectCount; i++)
  {
    Json::Value jsonObject = objects[i];
    auto objectDataPtr = LoadObjectDataFromJSON(jsonObject);
    levelData->objects.push_back(*objectDataPtr);
  }

  Json::Value targets = jsonObject["targets"];
  Json::ArrayIndex targetCount = targets.size();

  for( int i = 0; i < targetCount; i++)
  {
    Json::Value target = targets[i];
    int x = target["x"].asInt();
    int y = target["y"].asInt();
    levelData->targets.push_back(game_point(x, y));
  }

  return levelData;
}

std::unique_ptr<game_level_object_data> LoadObjectDataFromJSON(const Json::Value& jsonObject)
{
  auto objectDataPtr = std::make_unique<game_level_object_data>();

  // objectDataPtr->x = jsonObject["x"].asFloat();
  // objectDataPtr->y = jsonObject["y"].asFloat();

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

std::unique_ptr<game_level_data> LoadGameLevelData(const std::wstring& dataPath, const std::wstring& file)
{
  const std::wstring fullFilename = GetFullLevelFilename(dataPath, file);
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
  // const auto& object = *objectPtr;

  Json::Value root;
  
  // root["x"] = object.x;
  // root["y"] = object.x;
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

  fs::path saveFilename = gameLevelData.filename;
  fs::path backupFilename = saveFilename.parent_path();
  backupFilename /= "bak";
  backupFilename /= saveFilename.filename();

  // TODO: add backup

  std::ofstream ofs(gameLevelData.filename);
  ofs.write(jsonData.c_str(), jsonData.length());
  std::ofstream::iostate state = ofs.rdstate();
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
  fs::path path = dataPath;
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

  // TODO: get reduce working
  //
  // bool allSaved = std::reduce(saveLevelDataReturn.cbegin(), saveLevelDataReturn.end(), true,
  //   [](bool left, bool right)
  //   {
  //     return true;
  //   }
  // );

  return true;
}

std::wstring game_level_data_filenames::GetNext()
{
  return std::format(L"level_{:03}.json", ++filenameIndex);
}
