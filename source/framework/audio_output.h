#pragma once

class audio_output
{

public:

  static auto create(HWND wnd) -> void;
  static auto destroy() -> void;

  [[nodiscard]] static auto directSound() -> winrt::com_ptr<IDirectSound8>&;
  [[nodiscard]] static auto primarySoundBuffer() -> winrt::com_ptr<IDirectSoundBuffer>&;

private:

  static audio_output* m_instance;

  audio_output(HWND wnd);
  winrt::com_ptr<IDirectSound8> m_directSound;
  winrt::com_ptr<IDirectSoundBuffer> m_primarySoundBuffer;

};

[[nodiscard]] inline auto audio_output::directSound() -> winrt::com_ptr<IDirectSound8>&
{
  return m_instance->m_directSound;
}

[[nodiscard]] inline auto audio_output::primarySoundBuffer() -> winrt::com_ptr<IDirectSoundBuffer>&
{
  return m_instance->m_primarySoundBuffer;
}
