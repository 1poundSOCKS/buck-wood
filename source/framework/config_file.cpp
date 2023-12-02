#include "pch.h"
#include "config_file.h"

auto config_file::create(const wchar_t* filename) -> void
{
  m_configFile = new config_file(filename);
}

auto config_file::get_setting(const std::wstring& name) -> std::wstring
{
  return m_configFile->m_settings[name];
}

std::wstring config_file::ToWstr(std::string_view input)
{
  const auto maxOutputLength = 1024u;
  wchar_t output[maxOutputLength+1];
  auto outputLength = ::MultiByteToWideChar(CP_ACP, 0, input.data(), static_cast<int>(input.length()), output, maxOutputLength);
  output[outputLength] = L'\0';
  return output;
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

config_file::setting::setting(const std::wstring& text)
{
  size_t splitPos = text.find('=');
  key = text.substr(0, splitPos);
  value = text.substr(splitPos+1);
}
