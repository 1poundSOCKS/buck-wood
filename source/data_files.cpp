#include "data_files.h"

config_file::setting::setting(const std::wstring& text)
{
  int splitPos = text.find('=');
  key = text.substr(0, splitPos);
  value = text.substr(splitPos+1);
}

config_file::config_file(const wchar_t* filename)
{
  std::ifstream fileReader(filename, std::ifstream::in);
  if( fileReader.fail() ) throw L"error";

  std::string line;
  while( std::getline(fileReader, line) )
  {
    if( line.length() == 0 ) continue;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wideLine = converter.from_bytes(line);
    setting setting(wideLine);
    settings.insert(std::make_pair(setting.key, setting.value));
  }

  fileReader.close();
}

wav_file_header::wav_file_header(std::ifstream& reader)
{
  reader.read(reinterpret_cast<char*>(id), sizeof(id));
  reader.read(reinterpret_cast<char*>(&size), sizeof(size));
  reader.read(reinterpret_cast<char*>(format), sizeof(format));
}

wav_file_chunk::wav_file_chunk(std::ifstream& reader)
{
  reader.read(reinterpret_cast<char*>(id), sizeof(id));
  reader.read(reinterpret_cast<char*>(&size), sizeof(size));
  data = std::make_unique<uint8_t[]>(size);
  reader.read(reinterpret_cast<char*>(data.get()), size);
}

wav_file_data::wav_file_data(const wchar_t* filename)
{
  std::ifstream fileReader(filename, std::ifstream::binary);
  if( fileReader.fail() )
  {
    char* error = strerror(errno);
    throw L"error";
  }

  header = std::make_unique<wav_file_header>(fileReader);

  do { format = std::make_unique<wav_file_chunk>(fileReader); }
  while( memcmp(format->id, "fmt ", 4) );
  
  do { data = std::make_unique<wav_file_chunk>(fileReader); }
  while( memcmp(data->id, "data", 4) );
  
  memcpy(&dataFormat, format->data.get(), sizeof(dataFormat));
  
  fileReader.close();
}

int GetWavFileDataSize(const wav_file_data& wavFileData)
{
  return wavFileData.data->size;
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
