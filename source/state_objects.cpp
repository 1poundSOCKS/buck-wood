#include "state_objects.h"
#include "data_files.h"

game_level_data_ptr LoadGameLevelData(const std::wstring& dataPath, const std::wstring& file);
std::wstring GetFullLevelFilename(const std::wstring& dataPath, const std::wstring& file);

play_state::play_state()
{
}

game_state::game_state(const std::wstring& dataPath)
{
  gameData = std::make_shared<game_data>();

  gameData->push_back(LoadGameLevelData(dataPath, L"level_001.json"));
  gameData->push_back(LoadGameLevelData(dataPath, L"level_002.json"));
  gameData->push_back(LoadGameLevelData(dataPath, L"level_003.json"));
  gameData->push_back(LoadGameLevelData(dataPath, L"level_004.json"));

  playState = std::make_unique<play_state>();
}

game_level_data_ptr LoadGameLevelData(const std::wstring& dataPath, const std::wstring& file)
{
  level_data_json_ptr jsonLevelData = std::make_unique<level_data_json>(GetFullLevelFilename(dataPath, file).c_str());
  return LoadLevelDataFromJSON(*jsonLevelData);
}

std::wstring GetFullLevelFilename(const std::wstring& dataPath, const std::wstring& file)
{
  fs::path fullFilename = dataPath;
  fullFilename /= L"levels";
  fullFilename /= file;
  return fullFilename;
}
