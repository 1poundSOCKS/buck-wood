#ifndef _sound_buffers_
#define _sound_buffers_

#include "framework.h"

struct sound_buffers
{
  sound_buffers(IDirectSound8* directSound, const std::wstring& path);

  winrt::com_ptr<IDirectSoundBuffer8> menuTheme;
  winrt::com_ptr<IDirectSoundBuffer8> shoot;
  winrt::com_ptr<IDirectSoundBuffer8> thrust;
  winrt::com_ptr<IDirectSoundBuffer8> targetActivated;
};

winrt::com_ptr<IDirectSoundBuffer8> LoadSoundBuffer(IDirectSound8* directSound, const std::wstring& path, const std::wstring& file);

#endif
