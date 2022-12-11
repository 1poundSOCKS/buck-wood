#ifndef _play_screen_
#define _play_screen_

#include "screens/play/play_screen_state.h"
#include "screen_render_data.h"
#include "screen_sound_data.h"

void UpdateScreenState
(
  play_screen_state& screenState, 
  const screen_input_state& inputState
);

void RenderFrame
(
  ID2D1RenderTarget* renderTarget, 
  const bespoke_render_data& renderData, 
  const play_screen_state& screenState
);

void PlaySoundEffects
(
  const play_screen_state& screenState, 
  const bespoke_sound_data& soundData
);

bool ContinueRunning(const play_screen_state& screenState);

#endif
