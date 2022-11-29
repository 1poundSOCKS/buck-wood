#ifndef _direct_sound_
#define _direct_sound_

winrt::com_ptr<IDirectSound8> CreateDirectSound(HWND window);
winrt::com_ptr<IDirectSoundBuffer> CreatePrimarySoundBuffer(IDirectSound8* directSound);

#endif
