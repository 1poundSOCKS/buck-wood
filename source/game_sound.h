#ifndef _game_sound_
#define _game_sound_

#include "sound_buffers.h"
#include "state_objects.h"

void UpdateSound(const sound_buffers& soundBuffers, const game_state& gameState, const game_events& events);
void UpdateSound(const sound_buffers& soundBuffers, const play_state& playState, const game_events& events);

#endif
