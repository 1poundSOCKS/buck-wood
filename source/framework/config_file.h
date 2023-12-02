#pragma once

class config_file
{
public:

  static auto create(const wchar_t* filename) -> void;
  static auto get_setting(const std::wstring& name) -> std::wstring;

private:

  config_file(const wchar_t* filename);
  static auto ToWstr(std::string_view input) -> std::wstring;

private:

  inline static config_file* m_configFile { nullptr };

  struct setting
  {
    setting(const std::wstring& text);

    std::wstring key;
    std::wstring value;
  };

  using settings_collection = std::map<std::wstring, std::wstring>;
  settings_collection m_settings;
};
