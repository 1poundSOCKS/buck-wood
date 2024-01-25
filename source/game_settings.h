#pragma once

class game_settings
{

public:

  static auto load() -> void;
  static auto save() -> void;

  static auto setFramerate(std::optional<int> value) -> void;
  static auto setFullscreen(bool value) -> void;
  static auto setShowDiagnostics(bool value) -> void;
  static auto setEffectsVolume(int value) -> int;
  static auto setMusicVolume(int value) -> int;

  static [[nodiscard]] auto framerate() -> std::optional<int>;
  static [[nodiscard]] auto fullscreen() -> bool;
  static [[nodiscard]] auto showDiagnostics() -> bool;
  static [[nodiscard]] auto effectsVolume() -> int;
  static [[nodiscard]] auto musicVolume() -> int;

private:

  auto Load() -> void;
  auto Save() -> void;

private:

  static game_settings* m_instance;

  std::optional<int> m_framerate { 60 };
  bool m_fullscreen { true };
  bool m_showDiagnostics { false };
  int m_effectsVolume { 10 };
  int m_musicVolume { 10 };

};

inline auto game_settings::load() -> void
{
  if( m_instance == nullptr )
  {
    m_instance = new game_settings();
  }

  if( m_instance )
  {
    m_instance->Load();
  }
}

inline auto game_settings::save() -> void
{
  if( m_instance )
  {
    m_instance->Save();
  }
}

inline auto game_settings::setFramerate(std::optional<int> value) -> void
{
  m_instance->m_framerate = value;
}

inline auto game_settings::setFullscreen(bool value) -> void
{
  m_instance->m_fullscreen = value;
}

inline auto game_settings::setShowDiagnostics(bool value) -> void
{
  m_instance->m_showDiagnostics = value;
}

inline auto game_settings::setEffectsVolume(int value) -> int
{
  return m_instance ? m_instance->m_effectsVolume = value : value;
}

inline auto game_settings::setMusicVolume(int value) -> int
{
  return m_instance ? m_instance->m_musicVolume = value : value;
}

inline [[nodiscard]] auto game_settings::framerate() -> std::optional<int>
{
  return m_instance->m_framerate;
}

inline [[nodiscard]] auto game_settings::fullscreen() -> bool
{
  return m_instance->m_fullscreen;
}

inline [[nodiscard]] auto game_settings::showDiagnostics() -> bool
{
  return m_instance->m_showDiagnostics;
}

inline [[nodiscard]] auto game_settings::effectsVolume() -> int
{
  return m_instance ? m_instance->m_effectsVolume : 10;
}

inline [[nodiscard]] auto game_settings::musicVolume() -> int
{
  return m_instance ? m_instance->m_musicVolume : 10;
}
