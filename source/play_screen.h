#ifndef _play_screen_
#define _play_screen_

#include "screens/play/play_screen_state.h"

void UpdateScreenState(play_screen_state& screenState, const screen_input_state& inputState);
void RenderFrame(ID2D1RenderTarget* renderTarget, const play_screen_state& screenState);
void PlaySoundEffects(const play_screen_state& screenState);
bool ContinueRunning(const play_screen_state& screenState);

void FormatDiagnostics(const play_screen_state& screenState, auto diagnosticsDataInserter)
{
  diagnosticsDataInserter = screenState.GetMouseDiagnostics();
}

#endif
