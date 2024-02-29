#pragma once

class game_settings
{

public:

  enum frame_rate_type { capped, uncapped };

  static auto create() -> void;
  static auto destroy() -> void;

  static auto load() -> void;
  static auto save() -> void;

  static auto setFramerateCapped() -> void;
  static auto setFramerateUncapped() -> void;
  static auto setSwapChainRefreshRate(int value) -> void;
  static auto setRenderTargetWidth(int value) -> void;
  static auto setRenderTargetHeight(int value) -> void;
  static auto setFullscreen(bool value) -> void;
  static auto setShowDiagnostics(bool value) -> void;
  static auto setEffectsVolume(int value) -> int;
  static auto setMusicVolume(int value) -> int;

  static [[nodiscard]] auto framerateCapped() -> bool;
  static [[nodiscard]] auto framerateUncapped() -> bool;
  static [[nodiscard]] auto swapChainRefreshRate() -> int;
  static [[nodiscard]] auto renderTargetWidth() -> int;
  static [[nodiscard]] auto renderTargetHeight() -> int;
  static [[nodiscard]] auto fullscreen() -> bool;
  static [[nodiscard]] auto showDiagnostics() -> bool;
  static [[nodiscard]] auto effectsVolume() -> int;
  static [[nodiscard]] auto musicVolume() -> int;

private:

  auto Load() -> void;
  auto Save() -> void;

private:

  static game_settings* m_instance;

  frame_rate_type m_frameRateType { capped };
  bool m_fullscreen { true };
  int m_swapChainRefreshRate { 60 };
  int m_renderTargetWidth { 1920 };
  int m_renderTargetHeight { 1080 };
  bool m_showDiagnostics { false };
  int m_effectsVolume { 1 };
  int m_musicVolume { 1 };

};

inline auto game_settings::create() -> void
{
  destroy();
  m_instance = new game_settings();
}

inline auto game_settings::destroy() -> void
{
  delete m_instance;
  m_instance = nullptr;
}

inline auto game_settings::load() -> void
{
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

inline auto game_settings::setFramerateCapped() -> void
{
  m_instance->m_frameRateType = frame_rate_type::capped;
}

inline auto game_settings::setFramerateUncapped() -> void
{
  m_instance->m_frameRateType = frame_rate_type::uncapped;
}

inline auto game_settings::setSwapChainRefreshRate(int value) -> void
{
  m_instance->m_swapChainRefreshRate = value;
}

inline auto game_settings::setRenderTargetWidth(int value) -> void
{
  m_instance->m_renderTargetWidth = value;
}

inline auto game_settings::setRenderTargetHeight(int value) -> void
{
  m_instance->m_renderTargetHeight = value;
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

inline [[nodiscard]] auto game_settings::framerateCapped() -> bool
{
  return m_instance->m_frameRateType == frame_rate_type::capped;
}

inline [[nodiscard]] auto game_settings::framerateUncapped() -> bool
{
  return m_instance->m_frameRateType == frame_rate_type::uncapped;
}

inline auto game_settings::swapChainRefreshRate() -> int
{
  return m_instance->m_swapChainRefreshRate;
}

inline [[nodiscard]] auto game_settings::renderTargetWidth() -> int
{
  return m_instance->m_renderTargetWidth;
}

inline [[nodiscard]] auto game_settings::renderTargetHeight() -> int
{
  return m_instance->m_renderTargetHeight;
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
