#include "state_objects.h"
#include "data_files.h"

play_state::play_state()
{
}

game_state::game_state(const std::wstring& dataPath)
{
  gameData = std::make_shared<game_data>();

  fs::path jsonFilename = dataPath;
  jsonFilename /= L"levels";
  jsonFilename /= "test_level.json";
  level_data_json_ptr jsonLevelData = std::make_unique<level_data_json>(jsonFilename.c_str());
  game_level_data_ptr gameLevelData = LoadLevelDataFromJSON(*jsonLevelData);

  gameData->push_back(gameLevelData);
  gameData->push_back(CreateFirstGameLevelData());
  gameData->push_back(CreateSecondGameLevelData());
  gameData->push_back(CreateThirdGameLevelData());

  playState = std::make_unique<play_state>();
}
