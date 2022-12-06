#include "pch.h"
#include "level_edit_screen_state.h"
#include "screen_sound_data.h"

void PlaySoundEffects(const level_edit_screen_state& screenState, const bespoke_sound_data& soundData)
{
  const auto soundBuffers = global_sound_buffer_selector { soundData.soundBuffers };
}
