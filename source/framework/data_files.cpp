#include "pch.h"
#include "data_files.h"

std::wstring ToWstr(std::string_view input)
{
  const auto maxOutputLength = 1024u;
  wchar_t output[maxOutputLength+1];
  auto outputLength = ::MultiByteToWideChar(CP_ACP, 0, input.data(), static_cast<int>(input.length()), output, maxOutputLength);
  output[outputLength] = L'\0';
  return output;
}

config_file* config_file::m_configFile = nullptr;

auto config_file::create(const wchar_t* filename) -> void
{
  m_configFile = new config_file(filename);
}

auto config_file::getSetting(const std::wstring& name) -> std::wstring
{
  return m_configFile->m_settings[name];
}

config_file::setting::setting(const std::wstring& text)
{
  size_t splitPos = text.find('=');
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
    std::wstring wideLine = ToWstr(line);

    setting newSetting(wideLine);
    m_settings.insert(std::make_pair(newSetting.key, newSetting.value));
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
    throw std::format("error: {}", error);
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
