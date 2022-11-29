#ifndef _sound_buffer_
#define _sound_buffer_

#include "data_files.h"

winrt::com_ptr<IDirectSoundBuffer8> CreateSoundBuffer(IDirectSound8* directSound, const wav_file_data& data);

#endif
