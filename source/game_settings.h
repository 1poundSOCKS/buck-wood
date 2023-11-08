#pragma once

class game_settings
{

public:

  static auto load() -> void;
  static auto save() -> void;

  static auto setEffectsVolume(int value) -> void;
  static auto setMusicVolume(int value) -> void;

private:

  auto Load() -> void;
  auto Save() -> void;

private:

  static game_settings* m_instance;

  int m_effectsVolume { 8 };
  int m_musicVolume { 8 };

};

inline auto game_settings::load() -> void
{
  if( m_instance == nullptr )
  {
    m_instance = new game_settings();
  }

  m_instance->Load();
}

inline auto game_settings::save() -> void
{
  if( m_instance )
  {
    m_instance->Save();
  }
}

inline auto game_settings::setEffectsVolume(int value) -> void
{
  if( m_instance )
  {
    m_instance->m_effectsVolume = value;
  }
}

inline auto game_settings::setMusicVolume(int value) -> void
{
  if( m_instance )
  {
    m_instance->m_effectsVolume = value;
  }
}
